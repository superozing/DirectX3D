#include "pch.h"
#include "CVideo.h"

#include "CDevice.h"
IMFAttributes* CVideo::g_pMFAttributes = nullptr;
CVideo::CVideo()
	: CAsset(ASSET_TYPE::VIDEO, false)
{
}

CVideo::~CVideo()
{
	if (m_pReader)
		m_pReader->Release();
	if (m_pTexture)
		m_pTexture->Release();
	if (m_pShaderResourceView)
		m_pShaderResourceView->Release();

	m_decodingThread.join();
}

int CVideo::Load(const wstring& _strFilePath)
{
	HRESULT hr;

	// 소스 리더 생성
	hr = MFCreateSourceReaderFromURL(_strFilePath.c_str(), CVideo::g_pMFAttributes, &m_pReader);
	if (FAILED(hr))
	{
		// 에러 처리
		return E_FAIL;
	}

	// 비디오 스트림 선택
	hr = m_pReader->SetStreamSelection(MF_SOURCE_READER_ALL_STREAMS, FALSE);
	hr = m_pReader->SetStreamSelection(MF_SOURCE_READER_FIRST_VIDEO_STREAM, TRUE);

	// 비디오 포맷 설정
	IMFMediaType* pNativeType = NULL;
	IMFMediaType* pType		  = NULL;
	hr						  = m_pReader->GetNativeMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &pNativeType);
	if (SUCCEEDED(hr))
	{
		hr = MFCreateMediaType(&pType);
		hr = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
		hr = pType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
		hr = m_pReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, pType);

		// 비디오 정보 추출
		MFGetAttributeSize(pNativeType, MF_MT_FRAME_SIZE, &m_width, &m_height);
		UINT32 numerator, denominator;
		MFGetAttributeRatio(pNativeType, MF_MT_FRAME_RATE, &numerator, &denominator);
		m_frameRate = numerator / denominator;

		pNativeType->Release();
		pType->Release();
	}

	// 텍스처 생성
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width				  = m_width;
	desc.Height				  = m_height;
	desc.MipLevels			  = 1;
	desc.ArraySize			  = 1;
	desc.Format				  = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count	  = 1;
	desc.Usage				  = D3D11_USAGE_DYNAMIC;
	desc.BindFlags			  = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags		  = D3D11_CPU_ACCESS_WRITE;

	ID3D11Device* device = CDevice::GetInst()->GetDevice();
	hr					 = device->CreateTexture2D(&desc, NULL, &m_pTexture);
	if (FAILED(hr))
	{
		// 에러 처리
		return E_FAIL;
	}

	// 셰이더 리소스 뷰 생성
	hr = device->CreateShaderResourceView(m_pTexture, NULL, &m_pShaderResourceView);
	if (FAILED(hr))
	{
		// 에러 처리
		return E_FAIL;
	}

	// 비디오 길이 얻기
	PROPVARIANT var;
	PropVariantInit(&var);
	hr = m_pReader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &var);
	if (SUCCEEDED(hr))
	{
		m_duration = var.hVal.QuadPart;
		PropVariantClear(&var);
	}

	// 디코딩 스레드 시작

	// m_decodingThread = std::thread(&CVideo::DecodeThreadFunction, this);

	return S_OK;
}

int CVideo::Play()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (!m_isPlaying)
	{
		m_isPlaying	  = true;
		m_currentTime = 0; // 처음부터 재생
		// 디코딩 스레드가 아직 시작되지 않았다면 시작
		if (!m_decodingThread.joinable())
		{
			m_decodingThread = std::thread(&CVideo::DecodeThreadFunction, this);
		}
	}
	return S_OK;
}

void CVideo::Stop()
{
	std::lock_guard<mutex> lock(m_mutex);
	m_isPlaying	  = false;
	m_currentTime = 0;
}

void CVideo::Pause()
{
	std::lock_guard<mutex> lock(m_mutex);
	m_isPlaying = false;
}

void CVideo::Resume()
{
	std::lock_guard<mutex> lock(m_mutex);
	m_isPlaying = true;
}

void CVideo::UpdateData()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (!m_isPlaying || m_sampleQueue.empty())
	{
		return;
	}

	IMFSample* pSample = m_sampleQueue.front();
	m_sampleQueue.pop();

	// 샘플에서 버퍼 얻기
	IMFMediaBuffer* pBuffer = NULL;
	HRESULT			hr		= pSample->GetBufferByIndex(0, &pBuffer);
	if (SUCCEEDED(hr))
	{
		// 버퍼 잠그기
		BYTE* pBytes	= NULL;
		DWORD bufferLen = 0;
		hr				= pBuffer->Lock(&pBytes, NULL, &bufferLen);
		if (SUCCEEDED(hr))
		{
			// DirectX 텍스처 업데이트
			ID3D11DeviceContext*	 pContext = CDevice::GetInst()->GetContext();
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			hr = pContext->Map(m_pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (SUCCEEDED(hr))
			{
				memcpy(mappedResource.pData, pBytes, bufferLen);
				pContext->Unmap(m_pTexture, 0);
				pContext->VSSetShaderResources(0, 1, &m_pShaderResourceView);
				pContext->HSSetShaderResources(0, 1, &m_pShaderResourceView);
				pContext->DSSetShaderResources(0, 1, &m_pShaderResourceView);
				pContext->GSSetShaderResources(0, 1, &m_pShaderResourceView);
				pContext->PSSetShaderResources(0, 1, &m_pShaderResourceView);
			}

			pBuffer->Unlock();
		}
		pBuffer->Release();
	}
	pSample->Release();

	// 현재 시간 업데이트
	LONGLONG sampleTime = 0;
	hr					= pSample->GetSampleTime(&sampleTime);
	if (SUCCEEDED(hr))
	{
		m_currentTime = sampleTime;
	}

	// 비디오 끝에 도달했는지 확인
	if (m_currentTime >= m_duration)
	{
		if (m_isLooping)
		{
			m_currentTime = 0;
			// 리더를 시작 위치로 되감기
			m_pReader->SetCurrentPosition(GUID_NULL, {0});
		}
		else
		{
			m_isPlaying = false;
		}
	}
}

void CVideo::DecodeThreadFunction()
{
	while (m_isPlaying)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_sampleQueue.size() < 5) // 큐에 최대 5개의 프레임만 유지
		{
			IMFSample* pSample = NULL;
			HRESULT	   hr	   = GetNextFrame(&pSample);
			if (SUCCEEDED(hr) && pSample)
			{
				m_sampleQueue.push(pSample);
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

HRESULT CVideo::GetNextFrame(IMFSample** ppSample)
{
	HRESULT	 hr = S_OK;
	DWORD	 streamIndex, flags;
	LONGLONG timestamp;

	hr = m_pReader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &streamIndex, &flags, &timestamp, ppSample);

	return hr;
}

void CVideo::SetVolume(float volume)
{
	// 볼륨 설정 구현
}

void CVideo::SetLooping(bool isLooping)
{
	m_isLooping = isLooping;
}

float CVideo::GetProgress()
{
	return static_cast<float>(m_currentTime) / m_duration;
}

LONGLONG CVideo::GetDuration()
{
	return m_duration;
}
