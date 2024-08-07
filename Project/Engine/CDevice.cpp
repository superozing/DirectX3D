#include "pch.h"
#include "CDevice.h"

#include "CConstBuffer.h"
#include "CStructuredBuffer.h"
#include "CAssetMgr.h"
#include "CRenderMgr.h"
#include "CTaskMgr.h"

CDevice::CDevice()
	: m_hRenderWnd(nullptr)
	, m_arrCB{}
	, m_arrSB{}
	, m_arrRS{}
	, m_arrDS{}
	, m_arrBS{}
	, m_arrSampler{}
	, bIsWindowMode(false)
{
}

CDevice::~CDevice()
{
	Delete_Array(m_arrCB);

	Delete_Array(m_arrSB);
}

int CDevice::init(HWND _hWnd, Vec2 _vResolution, bool bWindowMode)
{
	// 출력 윈도우
	m_hRenderWnd = _hWnd;

	// 버퍼 해상도
	m_vRenderResolution = _vResolution;

	// 장치 초기화
	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0,
								 D3D11_SDK_VERSION, m_Device.GetAddressOf(), &eLevel, m_Context.GetAddressOf())))
	{
		MessageBox(nullptr, L"Device, Context 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// 스왚체인 생성
	if (FAILED(CreateSwapChain(bWindowMode)))
	{
		MessageBox(nullptr, L"SwapChain 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// 렌더타겟, 렌더타겟 뷰, 뎁스스텐실 타겟, 뎁스 스텐실 뷰 생성
	if (FAILED(CreateTargetView()))
	{
		MessageBox(nullptr, L"타겟 및 View 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateRasterizerState()))
	{
		MessageBox(nullptr, L"Rasterizer State 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateDepthStencilState()))
	{
		MessageBox(nullptr, L"DepthStencil State 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateBlendState()))
	{
		MessageBox(nullptr, L"Blend State 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateSamplerState()))
	{
		MessageBox(nullptr, L"Sampler State 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateConstBuffer()))
	{
		MessageBox(nullptr, L"상수버퍼 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateStructuredBuffer()))
	{
		MessageBox(nullptr, L"구조화 버퍼 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	CONTEXT->VSSetShader(nullptr, nullptr, 0);
	CONTEXT->HSSetShader(nullptr, nullptr, 0);
	CONTEXT->DSSetShader(nullptr, nullptr, 0);
	CONTEXT->GSSetShader(nullptr, nullptr, 0);
	CONTEXT->PSSetShader(nullptr, nullptr, 0);

	return S_OK;
}

void CDevice::Present()
{
	m_SwapChain->Present(0, 0);
}

int CDevice::RenewResolution(Vec2 _vResolution, bool bWindowMode)
{
	m_vRenderResolution = _vResolution;

	CRenderMgr::GetInst()->DeleteBlurTex();
	DeleteTexturesForResolutionChange();

	// 스왚체인 생성
	if (FAILED(CreateSwapChain()))
	{
		MessageBox(nullptr, L"SwapChain 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// 렌더타겟, 렌더타겟 뷰, 뎁스스텐실 타겟, 뎁스 스텐실 뷰 생성
	if (FAILED(CreateTargetView()))
	{
		MessageBox(nullptr, L"타겟 및 View 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	CRenderMgr::GetInst()->CreateBlurTex();
	CRenderMgr::GetInst()->ResetMRT();
	CRenderMgr::GetInst()->CreateMRT();

	// MRT와 연관없는 Texture 생성
	CAssetMgr::GetInst()->CreateTexture(L"CopyRTtex", m_vRenderResolution.x, m_vRenderResolution.y,
										DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

	CRenderMgr::GetInst()->m_PostProcessTex =
		CAssetMgr::GetInst()->CreateTexture(L"PostProcessTex", (UINT)m_vRenderResolution.x, (UINT)m_vRenderResolution.y,
											DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

	RematchMtrlTexParam();

	return S_OK;
}

int CDevice::CreateSwapChain(bool _bFullscreen)
{
	if (m_SwapChain)
	{
		m_SwapChain.Reset();
	}

	// SwapChain 생성 구조체
	DXGI_SWAP_CHAIN_DESC tDesc = {};
	ZeroMemory(&tDesc, sizeof(tDesc));

	// SwapChain 이 관리하는 Buffer(RenderTarget) 의 구성 정보
	tDesc.BufferCount						 = 1;
	tDesc.BufferUsage						 = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tDesc.BufferDesc.Width					 = (UINT)m_vRenderResolution.x;
	tDesc.BufferDesc.Height					 = (UINT)m_vRenderResolution.y;
	tDesc.BufferDesc.Format					 = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.BufferDesc.RefreshRate.Denominator = 1;
	tDesc.BufferDesc.RefreshRate.Numerator	 = 60;
	tDesc.BufferDesc.Scaling				 = DXGI_MODE_SCALING_UNSPECIFIED;
	tDesc.BufferDesc.ScanlineOrdering		 = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tDesc.Flags								 = 0;

	tDesc.SampleDesc.Count	 = 1;
	tDesc.SampleDesc.Quality = 0;

	tDesc.Windowed	   = _bFullscreen; // 창모드
	tDesc.OutputWindow = m_hRenderWnd; // SwapChain 의 출력 윈도우 지정

	// 스왚체인 생성기능을 가지고 있는 Factory 에 접근한다.
	ComPtr<IDXGIDevice>	 pIdxgiDevice = nullptr;
	ComPtr<IDXGIAdapter> pAdapter	  = nullptr;
	ComPtr<IDXGIFactory> pFactory	  = nullptr;

	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pIdxgiDevice.GetAddressOf());
	pIdxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	// SwapChain 생성
	if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &tDesc, m_SwapChain.GetAddressOf())))
	{
		return E_FAIL;
	}

	// 전역 데이터에 렌더링 해상도 기록
	g_global.g_RenderResolution = m_vRenderResolution;

	return S_OK;
}

int CDevice::CreateTargetView()
{
	// 렌더타겟 텍스쳐를 스왚체인으로부터 얻어온다.
	ComPtr<ID3D11Texture2D> tex2D;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)tex2D.GetAddressOf());

	// RenderTargetTexture 를 AssetMgr 에 등록
	CAssetMgr::GetInst()->CreateTexture(L"RenderTargetTex", tex2D);

	// DepthStencilTexture 생성
	CAssetMgr::GetInst()->CreateTexture(L"DepthStencilTex", (UINT)m_vRenderResolution.x, (UINT)m_vRenderResolution.y,
										DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

	return S_OK;
}

int CDevice::CreateRasterizerState()
{
	m_arrRS[(UINT)RS_TYPE::CULL_BACK] = nullptr;

	HRESULT result = S_OK;

	D3D11_RASTERIZER_DESC tDesc = {};

	tDesc.CullMode = D3D11_CULL_FRONT;
	tDesc.FillMode = D3D11_FILL_SOLID;
	result		   = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());
	if (FAILED(result))
		return E_FAIL;

	tDesc.CullMode = D3D11_CULL_NONE;
	tDesc.FillMode = D3D11_FILL_SOLID;
	result		   = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());
	if (FAILED(result))
		return E_FAIL;

	tDesc.CullMode = D3D11_CULL_NONE;
	tDesc.FillMode = D3D11_FILL_WIREFRAME;
	result		   = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());
	if (FAILED(result))
		return E_FAIL;

	return S_OK;
}

int CDevice::CreateDepthStencilState()
{
	HRESULT hr = S_OK;

	m_arrDS[(UINT)DS_TYPE::LESS] = nullptr;

	D3D11_DEPTH_STENCIL_DESC tDesc = {};

	// Less Equal
	tDesc.DepthEnable	 = true;
	tDesc.DepthFunc		 = D3D11_COMPARISON_LESS_EQUAL;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable	 = false;

	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());
	if (FAILED(hr))
		return E_FAIL;

	// Greater
	tDesc.DepthEnable	 = true;
	tDesc.DepthFunc		 = D3D11_COMPARISON_GREATER;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable	 = false;
	hr					 = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::GRATER].GetAddressOf());
	if (FAILED(hr))
		return E_FAIL;

	// Greater Equal
	tDesc.DepthEnable	 = true;
	tDesc.DepthFunc		 = D3D11_COMPARISON_GREATER_EQUAL;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable	 = false;
	hr					 = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::GRATER_EQUAL].GetAddressOf());
	if (FAILED(hr))
		return E_FAIL;

	// No Test
	tDesc.DepthEnable	 = false;
	tDesc.DepthFunc		 = D3D11_COMPARISON_NEVER;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable	 = false;
	hr					 = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_TEST].GetAddressOf());

	// No Write
	tDesc.DepthEnable	 = true;
	tDesc.DepthFunc		 = D3D11_COMPARISON_LESS;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	tDesc.StencilEnable	 = false;
	hr					 = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());
	if (FAILED(hr))
		return E_FAIL;

	// NoTest NoWrite
	tDesc.DepthEnable	 = false;
	tDesc.DepthFunc		 = D3D11_COMPARISON_NEVER;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	tDesc.StencilEnable	 = false;
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());
	if (FAILED(hr))
		return E_FAIL;

	// 오브젝트 그리기
	tDesc				   = {};
	tDesc.DepthEnable	   = true;
	tDesc.DepthFunc		   = D3D11_COMPARISON_LESS_EQUAL;
	tDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable	   = true;
	tDesc.StencilReadMask  = 0xFF;
	tDesc.StencilWriteMask = 0xFF;

	tDesc.FrontFace.StencilFunc		   = D3D11_COMPARISON_ALWAYS;
	tDesc.FrontFace.StencilPassOp	   = D3D11_STENCIL_OP_REPLACE;
	tDesc.FrontFace.StencilFailOp	   = D3D11_STENCIL_OP_KEEP;
	tDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	tDesc.BackFace.StencilFunc		  = D3D11_COMPARISON_ALWAYS;
	tDesc.BackFace.StencilPassOp	  = D3D11_STENCIL_OP_REPLACE;
	tDesc.BackFace.StencilFailOp	  = D3D11_STENCIL_OP_KEEP;
	tDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::STENCIL_WRITE].GetAddressOf());
	if (FAILED(hr))
		return E_FAIL;

	// 비트마스킹을 이용한 스텐실 테스트 설정
	D3D11_DEPTH_STENCIL_DESC tDescTestMask = {};
	tDescTestMask.DepthEnable			   = true;
	tDescTestMask.DepthFunc				   = D3D11_COMPARISON_LESS_EQUAL;
	tDescTestMask.DepthWriteMask		   = D3D11_DEPTH_WRITE_MASK_ALL;
	tDescTestMask.StencilEnable			   = true;
	tDescTestMask.StencilReadMask		   = 1 << (UINT)STENCIL_TYPE::GRAY; // 테스트할 비트 마스크 (2)
	tDescTestMask.StencilWriteMask		   = 0xFF;

	tDescTestMask.FrontFace.StencilFunc =
		D3D11_COMPARISON_NOT_EQUAL; // GRAY 비트가 미설정된 경우만 그냥통과(discard),설정된경우는 쉐이더로들어감
	tDescTestMask.FrontFace.StencilPassOp	   = D3D11_STENCIL_OP_KEEP;
	tDescTestMask.FrontFace.StencilFailOp	   = D3D11_STENCIL_OP_KEEP;
	tDescTestMask.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	tDescTestMask.BackFace.StencilFunc =
		D3D11_COMPARISON_NOT_EQUAL; // GRAY 비트가 미설정된 경우만 그냥통과(discard),설정된경우는 쉐이더로들어감
	tDescTestMask.BackFace.StencilPassOp	  = D3D11_STENCIL_OP_KEEP;
	tDescTestMask.BackFace.StencilFailOp	  = D3D11_STENCIL_OP_KEEP;
	tDescTestMask.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	hr = DEVICE->CreateDepthStencilState(&tDescTestMask, m_arrDS[(UINT)DS_TYPE::STENCIL_GRAY_TEST].GetAddressOf());
	if (FAILED(hr))
		return E_FAIL;

	//// BackFace Check
	// tDesc.DepthEnable = true;
	// tDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	// tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	// tDesc.StencilEnable = true;

	// tDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// tDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;

	// hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::BACKFACE_CHECK].GetAddressOf());
	// if (FAILED(hr)) return E_FAIL;

	//// FrontFace Check
	// tDesc.DepthEnable = true;
	// tDesc.DepthFunc = D3D11_COMPARISON_LESS;
	// tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	// tDesc.StencilEnable = true;

	// tDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//   tDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	//   tDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR;

	// hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::FRONTFACE_CHECK].GetAddressOf());
	// if (FAILED(hr)) return E_FAIL;

	//// Stencil Check
	// tDesc.DepthEnable = false;
	// tDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	// tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	// tDesc.StencilEnable = true;

	// tDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	// tDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
	// tDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;

	// hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::STENCIL_CHECK].GetAddressOf());
	// if (FAILED(hr)) return E_FAIL;

	return S_OK;
}

int CDevice::CreateBlendState()
{
	m_arrBS[(UINT)BS_TYPE::DEFAULT] = nullptr;

	D3D11_BLEND_DESC tDesc = {};

	// AlphaBlend
	tDesc.AlphaToCoverageEnable	 = false;
	tDesc.IndependentBlendEnable = false;

	tDesc.RenderTarget[0].BlendEnable = true;
	tDesc.RenderTarget[0].BlendOp	  = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlend	  = D3D11_BLEND_SRC_ALPHA;
	tDesc.RenderTarget[0].DestBlend	  = D3D11_BLEND_INV_SRC_ALPHA;

	tDesc.RenderTarget[0].BlendOpAlpha	 = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlendAlpha	 = D3D11_BLEND_ONE;
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());

	tDesc.AlphaToCoverageEnable	 = false;
	tDesc.IndependentBlendEnable = false;

	tDesc.RenderTarget[0].BlendEnable = true;
	tDesc.RenderTarget[0].BlendOp	  = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlend	  = D3D11_BLEND_ONE;
	tDesc.RenderTarget[0].DestBlend	  = D3D11_BLEND_ONE;

	tDesc.RenderTarget[0].BlendOpAlpha	 = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlendAlpha	 = D3D11_BLEND_ONE;
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());

	// Decal
	tDesc.AlphaToCoverageEnable	 = false;
	tDesc.IndependentBlendEnable = true;

	tDesc.RenderTarget[0].BlendEnable = true;
	tDesc.RenderTarget[0].BlendOp	  = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlend	  = D3D11_BLEND_SRC_ALPHA;
	tDesc.RenderTarget[0].DestBlend	  = D3D11_BLEND_INV_SRC_ALPHA;

	tDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	tDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ONE;
	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	tDesc.RenderTarget[1].BlendEnable = true;
	tDesc.RenderTarget[1].BlendOp	  = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[1].SrcBlend	  = D3D11_BLEND_ONE;
	tDesc.RenderTarget[1].DestBlend	  = D3D11_BLEND_ONE;

	tDesc.RenderTarget[1].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[1].SrcBlendAlpha			= D3D11_BLEND_ONE;
	tDesc.RenderTarget[1].DestBlendAlpha		= D3D11_BLEND_ONE;
	tDesc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::DECAL].GetAddressOf());

	return S_OK;
}

int CDevice::CreateSamplerState()
{
	D3D11_SAMPLER_DESC tDesc = {};

	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.Filter   = D3D11_FILTER_ANISOTROPIC;

	tDesc.MinLOD = 0;
	tDesc.MaxLOD = D3D11_FLOAT32_MAX;

	DEVICE->CreateSamplerState(&tDesc, m_arrSampler[0].GetAddressOf());

	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.Filter   = D3D11_FILTER_MIN_MAG_MIP_POINT;

	tDesc.MinLOD = 0;
	tDesc.MaxLOD = D3D11_FLOAT32_MAX;

	DEVICE->CreateSamplerState(&tDesc, m_arrSampler[1].GetAddressOf());

	CONTEXT->VSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->HSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->DSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->GSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->PSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->CSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());

	CONTEXT->VSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->HSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->DSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->GSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->PSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->CSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());

	return S_OK;
}

int CDevice::CreateConstBuffer()
{
	m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer(CB_TYPE::TRANSFORM);
	m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), 1);

	m_arrCB[(UINT)CB_TYPE::MATERIAL_CONST] = new CConstBuffer(CB_TYPE::MATERIAL_CONST);
	m_arrCB[(UINT)CB_TYPE::MATERIAL_CONST]->Create(sizeof(tMtrlConst), 1);

	m_arrCB[(UINT)CB_TYPE::ANIM2D_DATA] = new CConstBuffer(CB_TYPE::ANIM2D_DATA);
	m_arrCB[(UINT)CB_TYPE::ANIM2D_DATA]->Create(sizeof(tAnimData2D), 1);

	m_arrCB[(UINT)CB_TYPE::GLOBAL_DATA] = new CConstBuffer(CB_TYPE::GLOBAL_DATA);
	m_arrCB[(UINT)CB_TYPE::GLOBAL_DATA]->Create(sizeof(tGlobalData), 1);

	return S_OK;
}

#include "CStructuredBuffer.h"

int CDevice::CreateStructuredBuffer()
{
	m_arrSB[(UINT)SB_TYPE::LIGHT2D] = new CStructuredBuffer;
	m_arrSB[(UINT)SB_TYPE::LIGHT2D]->Create(sizeof(tLightInfo), 10, SB_READ_TYPE::READ_ONLY, true);

	m_arrSB[(UINT)SB_TYPE::LIGHT3D] = new CStructuredBuffer;
	m_arrSB[(UINT)SB_TYPE::LIGHT3D]->Create(sizeof(tLightInfo), 10, SB_READ_TYPE::READ_ONLY, true);

	m_arrSB[(UINT)SB_TYPE::CROSS] = new CStructuredBuffer;
	m_arrSB[(UINT)SB_TYPE::CROSS]->Create(sizeof(tRaycastOut), 1, SB_READ_TYPE::READ_WRITE, true);

	m_arrSB[(UINT)SB_TYPE::WEIGHTMAP] = new CStructuredBuffer;
	m_arrSB[(UINT)SB_TYPE::WEIGHTMAP]->Create(sizeof(tWeight_4), 1024 * 1024, SB_READ_TYPE::READ_WRITE, false);

	m_arrSB[(UINT)SB_TYPE::BLOOM] = new CStructuredBuffer;
	m_arrSB[(UINT)SB_TYPE::BLOOM]->Create(sizeof(tBloom), 1, SB_READ_TYPE::READ_ONLY, true);

	m_arrSB[(UINT)SB_TYPE::AFTERIMAGE] = new CStructuredBuffer;
	m_arrSB[(UINT)SB_TYPE::AFTERIMAGE]->Create(sizeof(AfterImageInfo), 1, SB_READ_TYPE::READ_ONLY, true);

	return S_OK;
}

void CDevice::DeleteTexturesForResolutionChange()
{
	CAssetMgr::GetInst()->DeleteAsset<CTexture>(L"RenderTargetTex");
	CAssetMgr::GetInst()->DeleteAsset<CTexture>(L"DepthStencilTex");

	CAssetMgr::GetInst()->DeleteAsset<CTexture>(L"ColorTargetTex");
	CAssetMgr::GetInst()->DeleteAsset<CTexture>(L"PositionTargetTex");
	CAssetMgr::GetInst()->DeleteAsset<CTexture>(L"NormalTargetTex");
	CAssetMgr::GetInst()->DeleteAsset<CTexture>(L"EmissiveTargetTex");

	CAssetMgr::GetInst()->DeleteAsset<CTexture>(L"DiffuseTargetTex");
	CAssetMgr::GetInst()->DeleteAsset<CTexture>(L"SpecularTargetTex");

	CAssetMgr::GetInst()->DeleteAsset<CTexture>(L"ShadowDepthTargetTex");
	CAssetMgr::GetInst()->DeleteAsset<CTexture>(L"ShadowDepthStencilTex");

	CAssetMgr::GetInst()->DeleteAsset<CTexture>(L"CopyRTtex");
	CAssetMgr::GetInst()->DeleteAsset<CTexture>(L"PostProcessTex");

	CAssetMgr::GetInst()->DeleteAsset<CTexture>(L"AfterImageTargetTex");
}

void CDevice::RematchMtrlTexParam()
{
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(MTRL_dirlight);
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"ShadowDepthTargetTex"));

	pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(MTRL_pointlight);
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"ShadowDepthTargetTex"));

	pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(MTRL_spotlight);
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex"));

	pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(MTRLOutLineMerge);
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"ColorTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"DiffuseTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"SpecularTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_3, CAssetMgr::GetInst()->FindAsset<CTexture>(L"EmissiveTargetTex"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_4, CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex"));

	pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(MTRL_decal);
	pMtrl->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
}

#include <dxgidebug.h>
#include <dxgi.h>

void CDevice::ReportLiveObjects()
{
	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL_11_0;

	HRESULT hr = D3D11CreateDevice(nullptr,					  // 기본 어댑터 사용
								   D3D_DRIVER_TYPE_HARDWARE,  // 하드웨어 드라이버 사용
								   nullptr,					  // 소프트웨어 드라이버 없음
								   D3D11_CREATE_DEVICE_DEBUG, // 디버그 플래그
								   nullptr,					  // 기본 피처 레벨 배열 사용
								   0,						  // 피처 레벨 배열 크기
								   D3D11_SDK_VERSION,		  // SDK 버전
								   m_Device.GetAddressOf(),	  // 생성된 디바이스
								   &eLevel,					  // 피처 레벨
								   m_Context.GetAddressOf()	  // 생성된 디바이스 컨텍스트
	);

	ID3D11Debug* debugDevice = nullptr;
	hr						 = m_Device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDevice));
	if (SUCCEEDED(hr))
	{
		debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		debugDevice->Release();
	}
	else
	{
		std::cerr << "Failed to get D3D11 debug interface." << std::endl;
	}
}
