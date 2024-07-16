﻿#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"

CTexture::CTexture(bool _Engine)
	: CAsset(ASSET_TYPE::TEXTURE, _Engine)
	, m_Desc{}
{
}

CTexture::~CTexture()
{
}

int CTexture::Save(const wstring& _strRelativePath)
{
	// 경로
	wstring strFilePath = CPathMgr::GetContentPath();
	strFilePath += _strRelativePath;

	// 캡쳐 (GPU -> Sys)
	CaptureTexture(DEVICE, CONTEXT, m_Tex2D.Get(), m_Image);

	HRESULT hr		= S_OK;
	size_t	arrsize = m_Image.GetMetadata().arraySize;

	// mipmap이 1 이상인 경우
	if (1 < m_Desc.MipLevels)
	{
		D3D11_TEXTURE2D_DESC texdesc = {};
		texdesc						 = m_Desc;
		texdesc.MipLevels			 = 1;

		ComPtr<ID3D11Texture2D> pCopyTex2D;
		HRESULT					hr = DEVICE->CreateTexture2D(&texdesc, nullptr, pCopyTex2D.GetAddressOf());

		for (int i = 0; i < (int)m_Desc.ArraySize; ++i)
		{
			// m_Tex2D ==> pCopyTex2D
			UINT iDestSubresIdx = D3D11CalcSubresource(0, i, 1);
			UINT iSrcSubresIdx	= D3D11CalcSubresource(0, i, MAX_MIP);

			CONTEXT->CopySubresourceRegion(pCopyTex2D.Get(), iDestSubresIdx, 0, 0, 0, m_Tex2D.Get(), iSrcSubresIdx,
										   nullptr);
		}

		ScratchImage tempimage;
		CaptureTexture(DEVICE, CONTEXT, pCopyTex2D.Get(), tempimage);

		if (1 < arrsize)
		{
			hr = SaveToDDSFile(tempimage.GetImages(), arrsize, tempimage.GetMetadata(), DDS_FLAGS::DDS_FLAGS_NONE,
							   strFilePath.c_str());
		}
		else
		{
			hr = SaveToWICFile(*(tempimage.GetImages()), WIC_FLAGS_NONE,
							   GetWICCodec(static_cast<WICCodecs>(WICCodecs::WIC_CODEC_PNG)), strFilePath.c_str());
		}
	}

	// MipMap이 1 레벨(원본만) 있는 경우, 캡쳐 후 바로 저장
	else
	{
		if (1 < arrsize)
		{
			hr = SaveToDDSFile(m_Image.GetImages(), arrsize, m_Image.GetMetadata(), DDS_FLAGS::DDS_FLAGS_NONE,
							   strFilePath.c_str());
		}
		else
		{
			hr = SaveToWICFile(*(m_Image.GetImages()), WIC_FLAGS_NONE,
							   GetWICCodec(static_cast<WICCodecs>(WICCodecs::WIC_CODEC_PNG)), strFilePath.c_str());
		}
	}

	return hr;
}

int CTexture::Load(const wstring& _strFilePath)
{
	return Load(_strFilePath, 1);
}

int CTexture::Load(const wstring& _strFilePath, int _iMipLevel)
{
	wchar_t strBuff[50] = {};
	_wsplitpath_s(_strFilePath.c_str(), 0, 0, 0, 0, 0, 0, strBuff, 50);
	wstring strExt = strBuff;

	HRESULT hRet = S_OK;

	if (strExt == L".dds" || strExt == L".DDS")
	{
		// .dds .DDS
		hRet = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_FORCE_RGB, nullptr, m_Image);
	}
	else if (strExt == L".tga" || strExt == L".TGA")
	{
		// .tga .TGA
		hRet = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
	}
	else
	{
		// .png .jpg .jpeg .bmp
		hRet = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (FAILED(hRet))
	{
		// wsprintf(strBuff, L"에러코드 : %d", hRet);
		//  MessageBox(nullptr, strBuff, L"텍스쳐 로딩 실패", MB_OK);
		return hRet;
	}

	// Texture2D 생성
	m_Desc.Format = m_Image.GetMetadata().format;

	if (m_Image.GetMetadata().IsCubemap())
	{
		m_Desc.MipLevels = 1; // 0 ==> 가능한 모든 밉맵이 저장 될 수 있는 공간이 만들어짐
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_Desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	}
	else
	{
		m_Desc.MipLevels = _iMipLevel; // MAX_MIP;	// 0 ==> 가능한 모든 밉맵이 저장 될 수 있는 공간이 만들어짐
		m_Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		m_Desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}

	m_Desc.ArraySize = m_Image.GetMetadata().arraySize;

	m_Desc.SampleDesc.Count	  = 1;
	m_Desc.SampleDesc.Quality = 0;

	m_Desc.Usage		  = D3D11_USAGE_DEFAULT;
	m_Desc.CPUAccessFlags = 0;

	m_Desc.Width  = m_Image.GetMetadata().width;
	m_Desc.Height = m_Image.GetMetadata().height;

	HRESULT hr = DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf());

	// 원본데이터(밉맵 레벨 0) 를 각 칸에 옮긴다.
	for (int i = 0; i < m_Desc.ArraySize; ++i)
	{
		// GPU 에 데이터 옮기기(밉맵 포함)
		UINT iSubresIdx = D3D11CalcSubresource(0, i, m_Desc.MipLevels);

		CONTEXT->UpdateSubresource(m_Tex2D.Get(), iSubresIdx, nullptr, m_Image.GetImage(0, i, 0)->pixels,
								   m_Image.GetImage(0, i, 0)->rowPitch, m_Image.GetImage(0, i, 0)->slicePitch);
	}

	// SRV 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC viewdesc = {};
	viewdesc.Format							 = m_Desc.Format;

	if (m_Image.GetMetadata().IsCubemap())
	{
		viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	}
	else if (1 < m_Desc.ArraySize)
	{
		viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	}
	else
	{
		viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	}

	if (1 < m_Desc.ArraySize)
	{
		viewdesc.Texture2DArray.ArraySize		= m_Desc.ArraySize;
		viewdesc.Texture2DArray.MipLevels		= m_Desc.MipLevels;
		viewdesc.Texture2DArray.MostDetailedMip = 0;
	}
	else
	{
		viewdesc.Texture2DArray.ArraySize  = 1;
		viewdesc.Texture2D.MipLevels	   = m_Desc.MipLevels;
		viewdesc.Texture2D.MostDetailedMip = 0;
	}

	DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &viewdesc, m_SRV.GetAddressOf());

	// 밉맵 생성
	if (false == m_Image.GetMetadata().IsCubemap())
	{
		CONTEXT->GenerateMips(m_SRV.Get());
	}

	m_Tex2D->GetDesc(&m_Desc);
}

int CTexture::Create(UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _Usage)
{
	// Texture 생성
	// 픽셀 포맷
	m_Desc.Format = _Format;

	// 텍스쳐 해상도
	m_Desc.Width  = _Width;
	m_Desc.Height = _Height;

	// 텍스쳐 용도
	m_Desc.BindFlags = _BindFlag;

	// CPU 접근
	m_Desc.Usage = _Usage;
	if (_Usage == D3D11_USAGE_DYNAMIC)
	{
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	// 샘플링
	m_Desc.SampleDesc.Count	  = 1;
	m_Desc.SampleDesc.Quality = 0;

	// 저퀄리티 버전의 사본 생성여부
	m_Desc.MipLevels = 1;
	m_Desc.MiscFlags = 0;
	m_Desc.ArraySize = 1;

	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
	{
		return E_FAIL;
	}

	// View 생성
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		if (FAILED(DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf())))
		{
			return E_FAIL;
		}
	}

	else
	{
		if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			if (FAILED(DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}

		if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}

		if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			if (FAILED(DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

int CTexture::Create(ComPtr<ID3D11Texture2D> _tex2D)
{
	assert(_tex2D.Get());

	m_Tex2D = _tex2D;
	m_Tex2D->GetDesc(&m_Desc);

	// View 생성
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		if (FAILED(DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf())))
		{
			return E_FAIL;
		}
	}

	else
	{
		if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			if (FAILED(DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}

		if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}

		if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			if (FAILED(DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

int CTexture::CreateArrayTexture(const vector<Ptr<CTexture>>& _vecTex, int _iMapLevel)
{
	m_Desc			 = _vecTex[0]->GetDesc();
	m_Desc.ArraySize = (UINT)_vecTex.size();
	m_Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_Desc.MipLevels = _iMapLevel;

	HRESULT hr = DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf());

	if (FAILED(hr))
		return hr;

	// 원본데이터(밉맵 레벨 0) 를 각 칸에 옮긴다.
	for (int i = 0; i < _vecTex.size(); ++i)
	{
		UINT iOffset = D3D11CalcSubresource(0, i, _iMapLevel);
		CONTEXT->UpdateSubresource(m_Tex2D.Get(), iOffset, nullptr, _vecTex[i]->GetSysMem(), _vecTex[i]->GetRowPitch(),
								   _vecTex[i]->GetSlicePitch());
	}

	// Shader Resource View 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC viewdesc = {};
	viewdesc.Format							 = m_Desc.Format;
	viewdesc.ViewDimension					 = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewdesc.Texture2DArray.MipLevels		 = _iMapLevel;
	viewdesc.Texture2DArray.MostDetailedMip	 = 0;
	viewdesc.Texture2DArray.ArraySize		 = _vecTex.size();

	DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &viewdesc, m_SRV.GetAddressOf());

	// mipamp 생성
	CONTEXT->GenerateMips(m_SRV.Get());
	m_Tex2D->GetDesc(&m_Desc);

	return hr;
}

void CTexture::GenerateMip(UINT _iMipLevel)
{
	m_Tex2D = nullptr;
	m_SRV	= nullptr;
	m_RTV	= nullptr;
	m_DSV	= nullptr;
	m_UAV	= nullptr;

	// Texture2D 생성
	D3D11_TEXTURE2D_DESC tDesc = {};
	tDesc.Format			   = m_Image.GetMetadata().format;

	if (m_Image.GetMetadata().IsCubemap())
	{
		tDesc.MipLevels = 1; // 0 ==> 가능한 모든 밉맵이 저장 될 수 있는 공간이 만들어짐
		tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		tDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	}
	else
	{
		tDesc.MipLevels = _iMipLevel; // MAX_MIP;	// 0 ==> 가능한 모든 밉맵이 저장 될 수 있는 공간이 만들어짐
		tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		tDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}

	tDesc.ArraySize = m_Image.GetMetadata().arraySize;

	tDesc.SampleDesc.Count	 = 1;
	tDesc.SampleDesc.Quality = 0;

	tDesc.Usage			 = D3D11_USAGE_DEFAULT;
	tDesc.CPUAccessFlags = 0;

	tDesc.Width	 = m_Image.GetMetadata().width;
	tDesc.Height = m_Image.GetMetadata().height;

	HRESULT hr = DEVICE->CreateTexture2D(&tDesc, nullptr, m_Tex2D.GetAddressOf());

	// 원본데이터(밉맵 레벨 0) 를 각 칸에 옮긴다.
	for (int i = 0; i < tDesc.ArraySize; ++i)
	{
		// GPU 에 데이터 옮기기(밉맵 포함)
		UINT iSubresIdx = D3D11CalcSubresource(0, i, tDesc.MipLevels);

		CONTEXT->UpdateSubresource(m_Tex2D.Get(), iSubresIdx, nullptr, m_Image.GetImage(0, i, 0)->pixels,
								   m_Image.GetImage(0, i, 0)->rowPitch, m_Image.GetImage(0, i, 0)->slicePitch);
	}

	// SRV 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC viewdesc = {};
	viewdesc.Format							 = tDesc.Format;

	if (m_Image.GetMetadata().IsCubemap())
	{
		viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	}
	else if (1 < tDesc.ArraySize)
	{
		viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	}
	else
	{
		viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	}

	if (1 < tDesc.ArraySize)
	{
		viewdesc.Texture2DArray.ArraySize		= tDesc.ArraySize;
		viewdesc.Texture2DArray.MipLevels		= tDesc.MipLevels;
		viewdesc.Texture2DArray.MostDetailedMip = 0;
	}
	else
	{
		viewdesc.Texture2DArray.ArraySize  = 1;
		viewdesc.Texture2D.MipLevels	   = tDesc.MipLevels;
		viewdesc.Texture2D.MostDetailedMip = 0;
	}

	DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &viewdesc, m_SRV.GetAddressOf());

	// 밉맵 생성
	if (false == m_Image.GetMetadata().IsCubemap())
	{
		CONTEXT->GenerateMips(m_SRV.Get());
	}

	m_Tex2D->GetDesc(&m_Desc);
}

void CTexture::UpdateData(int _RegisterNum)
{
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

int CTexture::UpdateData_CS_SRV(int _RegisterNum)
{
	if (nullptr == m_SRV)
		return E_FAIL;

	m_RecentNum_SRV = _RegisterNum;

	CONTEXT->CSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	return S_OK;
}

int CTexture::UpdateData_CS_UAV(int _RegisterNum)
{
	if (nullptr == m_UAV)
		return E_FAIL;

	m_RecentNum_UAV = _RegisterNum;

	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, m_UAV.GetAddressOf(), &i);
	return S_OK;
}

void CTexture::Clear(int _RegisterNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	CONTEXT->VSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, &pSRV);
}

void CTexture::Clear_CS_SRV()
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	CONTEXT->CSSetShaderResources(m_RecentNum_SRV, 1, &pSRV);
}

void CTexture::Clear_CS_UAV()
{
	ID3D11UnorderedAccessView* pUAV = nullptr;

	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(m_RecentNum_UAV, 1, &pUAV, &i);
}

tPixel* CTexture::GetPixels()
{
	if (nullptr == m_Image.GetPixels())
	{
		CaptureTexture(DEVICE, CONTEXT, m_Tex2D.Get(), m_Image);
	}

	return (tPixel*)m_Image.GetPixels();
}