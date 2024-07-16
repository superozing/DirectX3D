#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CAssetMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CStructuredBuffer.h"
#include "CMRT.h"

void (*CRenderMgr::CameraChange)(CCamera*) = nullptr;

void CRenderMgr::init()
{
	m_Light2DBuffer = CDevice::GetInst()->GetStructuredBuffer(SB_TYPE::LIGHT2D);
	m_Light3DBuffer = CDevice::GetInst()->GetStructuredBuffer(SB_TYPE::LIGHT3D);

	m_pDebugObj = new CGameObject;
	m_pDebugObj->AddComponent(new CTransform);
	m_pDebugObj->AddComponent(new CMeshRender);

	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
	m_PostProcessTex =
		CAssetMgr::GetInst()->CreateTexture(L"PostProcessTex", (UINT)vRenderResolution.x, (UINT)vRenderResolution.y,
											DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

	// Noise Texture Load
	m_vecNoiseTex.push_back(
		CAssetMgr::GetInst()->Load<CTexture>(L"texture\\noise\\noise_01.png", L"texture\\noise\\noise_01.png"));
	m_vecNoiseTex.push_back(
		CAssetMgr::GetInst()->Load<CTexture>(L"texture\\noise\\noise_02.png", L"texture\\noise\\noise_02.png"));
	m_vecNoiseTex.push_back(
		CAssetMgr::GetInst()->Load<CTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_03.jpg"));

	m_vecNoiseTex[2]->UpdateData(14);
	m_vecNoiseTex[2]->UpdateData_CS_SRV(14);
	g_global.g_NoiseTexResolution = Vec2(m_vecNoiseTex[2]->GetWidth(), m_vecNoiseTex[2]->GetHeight());

	// MRT Create
	CreateBlurTex();
	CreateMRT();
}

void CRenderMgr::CreateMRT()
{

	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

	// =============
	// SwapChain MRT
	// =============
	{
		Ptr<CTexture> RTTex		  = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
		Ptr<CTexture> DSTex		  = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
		Vec4		  vClearColor = (Vec4(0.3f, 0.3f, 0.3f, 1.f));

		if (m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN] == nullptr)
			m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN] = new CMRT;

		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Create(&RTTex, 1, DSTex);
		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->SetClearColor(&vClearColor, 1);
	}

	// ============
	// Deferred MRT
	// ============
	{
		Ptr<CTexture> pRTTex[5] = {
			CAssetMgr::GetInst()->CreateTexture(L"ColorTargetTex", vResolution.x, vResolution.y,
												DXGI_FORMAT_R8G8B8A8_UNORM,
												D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->CreateTexture(L"PositionTargetTex", vResolution.x, vResolution.y,
												DXGI_FORMAT_R32G32B32A32_FLOAT,
												D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->CreateTexture(L"NormalTargetTex", vResolution.x, vResolution.y,
												DXGI_FORMAT_R32G32B32A32_FLOAT,
												D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->CreateTexture(L"EmissiveTargetTex", vResolution.x, vResolution.y,
												DXGI_FORMAT_R32G32B32A32_FLOAT,
												D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->FindAsset<CTexture>(L"RelativeLuminanceTargetTex"),
		};

		Vec4 arrClearColor[5] = {
			Vec4(0.f, 0.f, 0.f, 1.f), Vec4(0.f, 0.f, 0.f, -1.f), Vec4(0.f, 0.f, 0.f, 1.f),
			Vec4(0.f, 0.f, 0.f, 1.f), Vec4(0.f, 0.f, 0.f, 1.f),
		};

		Ptr<CTexture> DSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");

		if (m_arrMRT[(UINT)MRT_TYPE::DEFERRED] == nullptr)
			m_arrMRT[(UINT)MRT_TYPE::DEFERRED] = new CMRT;

		m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->Create(pRTTex, 5, DSTex);
		m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->SetClearColor(arrClearColor, 5);
	}

	// ============
	// Light MRT
	// ============
	{
		Ptr<CTexture> pRTTex[2] = {
			CAssetMgr::GetInst()->CreateTexture(L"DiffuseTargetTex", vResolution.x, vResolution.y,
												DXGI_FORMAT_R8G8B8A8_UNORM,
												D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->CreateTexture(L"SpecularTargetTex", vResolution.x, vResolution.y,
												DXGI_FORMAT_R8G8B8A8_UNORM,
												D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
		};

		Vec4 arrClearColor[2] = {
			Vec4(0.f, 0.f, 0.f, 1.f),
			Vec4(0.f, 0.f, 0.f, 1.f),
		};

		if (m_arrMRT[(UINT)MRT_TYPE::LIGHT] == nullptr)
			m_arrMRT[(UINT)MRT_TYPE::LIGHT] = new CMRT;

		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->Create(pRTTex, 2, nullptr);
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->SetClearColor(arrClearColor, 2);
	}

	// =========
	// Decal MRT
	// =========
	{
		Ptr<CTexture> pRTTex[2] = {
			CAssetMgr::GetInst()->FindAsset<CTexture>(L"ColorTargetTex"),
			CAssetMgr::GetInst()->FindAsset<CTexture>(L"EmissiveTargetTex"),
		};

		Vec4 arrClearColor[2] = {
			Vec4(0.f, 0.f, 0.f, 1.f),
			Vec4(0.f, 0.f, 0.f, 1.f),
		};

		if (m_arrMRT[(UINT)MRT_TYPE::DECAL] == nullptr)
			m_arrMRT[(UINT)MRT_TYPE::DECAL] = new CMRT;

		m_arrMRT[(UINT)MRT_TYPE::DECAL]->Create(pRTTex, 2, nullptr);
		m_arrMRT[(UINT)MRT_TYPE::DECAL]->SetClearColor(arrClearColor, 2);
	}

	// ============
	// SHADOW_DEPTH
	// ============
	{
		Ptr<CTexture> pRTTex[1] = {
			CAssetMgr::GetInst()->CreateTexture(L"ShadowDepthTargetTex", 8192, 8192, DXGI_FORMAT_R32_FLOAT,
												D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE)};

		Vec4 arrClearColor[1] = {
			Vec4(1.f, 0.f, 0.f, 0.f),
		};

		Ptr<CTexture> pDepthTex = CAssetMgr::GetInst()->CreateTexture(L"ShadowDepthStencilTex", 8192, 8192,
																	  DXGI_FORMAT_D32_FLOAT, D3D11_BIND_DEPTH_STENCIL);

		if (m_arrMRT[(UINT)MRT_TYPE::SHADOW_DEPTH] == nullptr)
			m_arrMRT[(UINT)MRT_TYPE::SHADOW_DEPTH] = new CMRT;

		m_arrMRT[(UINT)MRT_TYPE::SHADOW_DEPTH]->Create(pRTTex, 1, pDepthTex);
		m_arrMRT[(UINT)MRT_TYPE::SHADOW_DEPTH]->SetClearColor(arrClearColor, 1);
	}
}

void CRenderMgr::CopyRenderTargetToPostProcessTarget()
{
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

Ptr<CTexture> CRenderMgr::CopyRTTex(Ptr<CTexture> pTexture)
{
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");

	pTexture = CAssetMgr::GetInst()->FindAsset<CTexture>(L"CopyRTtex");

	CONTEXT->CopyResource(pTexture->GetTex2D().Get(), pRTTex->GetTex2D().Get());

	return pTexture;
}

void CRenderMgr::CopyFromTextureToTexture(Ptr<CTexture> pToTexture, Ptr<CTexture> pFromTexture)
{
	// 텍스처가 유효한지 확인
	if (pToTexture.Get() && pFromTexture.Get())
	{
		// 텍스처 형식 및 크기 확인
		D3D11_TEXTURE2D_DESC toDesc;
		pToTexture->GetTex2D()->GetDesc(&toDesc);

		D3D11_TEXTURE2D_DESC fromDesc;
		pFromTexture->GetTex2D()->GetDesc(&fromDesc);

		if (toDesc.Format == fromDesc.Format && toDesc.Width == fromDesc.Width && toDesc.Height == fromDesc.Height)
		{
			CONTEXT->CopyResource(pToTexture->GetTex2D().Get(), pFromTexture->GetTex2D().Get());
		}
		else
		{
			int a = 0;
			// 형식 또는 크기가 일치하지 않음
		}
	}
}

void CRenderMgr::CreateBlurTex()
{
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

	CAssetMgr::GetInst()->CreateTexture(
		L"RelativeLuminanceTargetTex", vResolution.x, vResolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, D3D11_USAGE_DEFAULT);

	CAssetMgr::GetInst()->CreateTexture(L"RelativeLuminanceCopyTex", (UINT)vResolution.x, (UINT)vResolution.y,
										DXGI_FORMAT_R32G32B32A32_FLOAT,
										D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	// 1~ 최대9
	for (int i = 1; i <= MAXBLURLEVEL; ++i)
	{
		int div = pow(2, i);

		// Bloom Down Texture
		Ptr<CTexture> pBloomOneTex = CAssetMgr::GetInst()->CreateTexture(
			L"BlurOne" + std::to_wstring(i), vResolution.x / div, vResolution.y / div,
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS, D3D11_USAGE_DEFAULT);
		Ptr<CTexture> pBloomTwoTex = CAssetMgr::GetInst()->CreateTexture(
			L"BlurTwo" + std::to_wstring(i), vResolution.x / div, vResolution.y / div,
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS, D3D11_USAGE_DEFAULT);

		m_vecBlurOneTex.push_back(pBloomOneTex);
		m_vecBlurTwoTex.push_back(pBloomTwoTex);
	}
}

void CRenderMgr::DeleteBlurTex()
{
}
