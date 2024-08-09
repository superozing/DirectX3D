#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CStructuredBuffer.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CDevice.h"
#include "CAssetMgr.h"
#include "components.h"

#include "CMRT.h"

CRenderMgr::CRenderMgr()
	: m_arrMRT{}
	, m_pDebugObj(nullptr)
	, m_DebugPosition(true)
	, m_EditorCam(nullptr)
	, m_RenderFunc(nullptr)
	, m_vClearColor(Vec4(0.f, 0.f, 0.f, 1.f))
	, m_Light2DBuffer(nullptr)
	, m_Light3DBuffer(nullptr)
	, m_bEscape(false)
{
	m_RenderFunc = &CRenderMgr::render_play;

	m_VignetteInfo.bVignetteRender = false;
	m_VignetteInfo.fAlpha		   = VignetteDuration;
	m_VignetteInfo.fDuration	   = VignetteAlpha;
	m_VignetteInfo.iVignettePower  = 1;
}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_pDebugObj)
		delete m_pDebugObj;

	Delete_Array(m_arrMRT);
}

void CRenderMgr::tick()
{
	// 렌더타겟 및 깊이 타겟 설정
	ClearMRT();
	UpdateData();

	CheckEscape();

	(this->*m_RenderFunc)();

	if (m_DebugPosition)
		render_debug();
	else
		m_DbgShapeInfo.clear();

	Clear();
}

void CRenderMgr::exit()
{
	ClearCamera();
}

void CRenderMgr::ClearMRT()
{
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Clear();
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->ClearRT();
	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->ClearRT();
	m_arrMRT[(UINT)MRT_TYPE::SHADOW_DEPTH]->Clear();
	m_arrMRT[(UINT)MRT_TYPE::AFTER_IMAGE]->ClearRT();
}

void CRenderMgr::ResetMRT()
{
	for (int i = 0; i < (UINT)MRT_TYPE::END; ++i)
	{
		delete m_arrMRT[i];
		m_arrMRT[i] = nullptr;
	}
}

void CRenderMgr::CreateDynamicShadowDepth()
{
	GetMRT(MRT_TYPE::SHADOW_DEPTH)->OMSet();

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		// 광원 시점에서 물체들의 깊이를 그린다.
		m_vecLight3D[i]->render_shadowdepth();
	}
}

void CRenderMgr::render_play()
{
	if (m_vecCam.empty())
		return;

	if (m_bGlobalShadowRender)
		CreateDynamicShadowDepth();

	// 메인 카메라 시점 렌더링
	if (nullptr != m_vecCam[0])
	{
		CCamera* pMainCam = m_vecCam[0];

		// 도메인에 따른 물체 분류
		pMainCam->SortObject();

		// 계산한 view 행렬과 proj 행렬을 전역 변수에 담는다.
		g_Transform.matView	   = pMainCam->GetViewMat();
		g_Transform.matViewInv = pMainCam->GetViewInvMat();
		g_Transform.matProj	   = pMainCam->GetProjMat();
		g_Transform.matProjInv = pMainCam->GetProjInvMat();

		// Domain 순서대로 렌더링
		// Deferred 물체 렌더링
		GetMRT(MRT_TYPE::DEFERRED)->OMSet();
		pMainCam->render_deferred();

		// Decal 물체 렌더링
		GetMRT(MRT_TYPE::DECAL)->OMSet();
		pMainCam->render_decal();

		// After Image 렌더링
		GetMRT(MRT_TYPE::AFTER_IMAGE)->OMSet();
		pMainCam->render_afterimage();

		// 그림자 판정

		// 광원처리
		// LightMRT 변경
		GetMRT(MRT_TYPE::LIGHT)->OMSet();

		// 광원이 자신의 영향 범위에 있는 Deferred 물체에 빛을 남긴다
		for (size_t i = 0; i < m_vecLight3D.size(); ++i)
		{
			m_vecLight3D[i]->render();
		}

		// Deferred + 광원 => SwapChain 으로 병합
		pMainCam->Merge();

		// Foward 렌더링
		pMainCam->render_forward();

		// 후처리 작업
		pMainCam->render_postprocess();
	}

	// 추가 보조카메라 시점 렌더링
	for (int i = 1; i < m_vecCam.size(); ++i)
	{
		g_Transform.matView	   = m_vecCam[i]->GetViewMat();
		g_Transform.matViewInv = m_vecCam[i]->GetViewInvMat();
		g_Transform.matProj	   = m_vecCam[i]->GetProjMat();
		g_Transform.matProjInv = m_vecCam[i]->GetProjInvMat();

		// 보조카메라 시점 기준 SortObject
		m_vecCam[i]->SortObject();

		// Foward 렌더링
		m_vecCam[i]->render_forward();
	}
}

void CRenderMgr::render_editor()
{
	if (nullptr == m_EditorCam)
		return;

	// 광원 시점에서 ShadowDepthMap 생성
	CreateDynamicShadowDepth();

	// 도메인에 따른 물체 분류
	m_EditorCam->SortObject();

	// 계산한 view 행렬과 proj 행렬을 전역변수에 담아둔다.
	g_Transform.matView	   = m_EditorCam->GetViewMat();
	g_Transform.matViewInv = m_EditorCam->GetViewInvMat();
	g_Transform.matProj	   = m_EditorCam->GetProjMat();
	g_Transform.matProjInv = m_EditorCam->GetProjInvMat();

	// Domain 순서대로 렌더링
	// Deferred 물체 렌더링
	GetMRT(MRT_TYPE::DEFERRED)->OMSet();
	m_EditorCam->render_deferred();

	// Decal 물체 렌더링
	GetMRT(MRT_TYPE::DECAL)->OMSet();
	m_EditorCam->render_decal();

	// 광원처리
	// Light MRT 로 변경
	GetMRT(MRT_TYPE::LIGHT)->OMSet();

	// 광원이 자신의 영향범위에 있는 Deferred 물체에 빛을 남긴다.
	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		m_vecLight3D[i]->render();
	}

	// Deferred + 광원 => SwapChain 으로 병합
	m_EditorCam->Merge();

	// Foward 렌더링
	m_EditorCam->render_forward();

	// 후처리 작업
	m_EditorCam->render_postprocess();
}

void CRenderMgr::render_debug()
{
	if (m_DbgShapeInfo.size() == 0)
		return;

	// 레벨이 플레이상태일경우(랜더Func가 render_play 일 경우)
	if (&CRenderMgr::render_play == m_RenderFunc)
	{
		if (m_vecCam.empty())
			return;

		g_Transform.matView = m_vecCam[0]->GetViewMat();
		g_Transform.matProj = m_vecCam[0]->GetProjMat();
	}
	// 레벨이 플레이상태가 아닐경우
	else
	{
		if (m_EditorCam == nullptr)
			return;

		g_Transform.matView = m_EditorCam->GetViewMat();
		g_Transform.matProj = m_EditorCam->GetProjMat();
	}

	list<tDebugShapeInfo>::iterator iter	   = m_DbgShapeInfo.begin();
	auto							cam		   = GetMainCam();
	UINT							LayerCheck = cam->GetLayerCheck();
	for (; iter != m_DbgShapeInfo.end();)
	{

		switch ((*iter).eShape)
		{
		case DEBUG_SHAPE::RECT:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrectdebug));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHcircledebug));
			break;
		case DEBUG_SHAPE::CROSS:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHcross));
			break;
		case DEBUG_SHAPE::CYLINDER:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHcylinder));
			break;

		case DEBUG_SHAPE::CUBE:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHcubedebug));
			break;
		case DEBUG_SHAPE::SPHERE:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHsphere));
			break;
		case DEBUG_SHAPE::CONE:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHcone));
		default:
			break;
		}

		m_pDebugObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"), 0);
		m_pDebugObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, (*iter).vColor);

		// 깊이판정 옵션 설정
		if ((*iter).bDepthTest)
			m_pDebugObj->MeshRender()->GetMaterial(0)->GetShader()->SetDSType(DS_TYPE::NO_WRITE);
		else
			m_pDebugObj->MeshRender()->GetMaterial(0)->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		D3D11_PRIMITIVE_TOPOLOGY PrevTopology = m_pDebugObj->MeshRender()->GetMaterial(0)->GetShader()->GetTopology();
		if (DEBUG_SHAPE::CROSS == (*iter).eShape)
		{
			m_pDebugObj->MeshRender()->GetMaterial(0)->GetShader()->SetTopology(
				D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		}
		else if (DEBUG_SHAPE::CYLINDER == (*iter).eShape)
		{
			m_pDebugObj->MeshRender()->GetMaterial(0)->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_pDebugObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, 3);
		}
		else if (DEBUG_SHAPE::SPHERE == (*iter).eShape)
		{
			m_pDebugObj->MeshRender()->GetMaterial(0)->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_pDebugObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, 1);
		}
		else
		{
			m_pDebugObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, 0);
		}

		m_pDebugObj->Transform()->SetWorldMat((*iter).matWorld);
		m_pDebugObj->Transform()->UpdateData();

		if (LayerCheck & 1 << iter->iLayer)
			m_pDebugObj->render();

		m_pDebugObj->MeshRender()->GetMaterial(0)->GetShader()->SetTopology(PrevTopology);

		(*iter).fLifeTime += DT;
		if ((*iter).fDuration <= (*iter).fLifeTime)
		{
			iter = m_DbgShapeInfo.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CRenderMgr::UpdateData()
{
	g_global.g_Light2DCount = (int)m_vecLight2D.size();
	g_global.g_Light3DCount = (int)m_vecLight3D.size();

	// 전역 데이터 업데이트
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL_DATA);
	pCB->SetData(&g_global);

	pCB->UpdateData();
	pCB->UpdateData_CS();

	// 2D 광원정보 업데이트
	static vector<tLightInfo> vecLight2DInfo;

	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		const tLightInfo& info = m_vecLight2D[i]->GetLightInfo();
		vecLight2DInfo.push_back(info);
	}

	if (!vecLight2DInfo.empty())
	{
		m_Light2DBuffer->SetData(vecLight2DInfo.data(), (UINT)vecLight2DInfo.size());
	}
	m_Light2DBuffer->UpdateData(11);

	vecLight2DInfo.clear();

	// 3D 광원정보 업데이트
	static vector<tLightInfo> vecLight3DInfo;

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		const tLightInfo& info = m_vecLight3D[i]->GetLightInfo();
		vecLight3DInfo.push_back(info);
	}

	if (!vecLight3DInfo.empty())
	{
		m_Light3DBuffer->SetData(vecLight3DInfo.data(), (UINT)vecLight3DInfo.size());
	}
	m_Light3DBuffer->UpdateData(12);

	vecLight3DInfo.clear();
}

void CRenderMgr::Clear()
{
	m_vecLight2D.clear();
	m_vecLight3D.clear();
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _Idx)
{
	if (_Idx == -1)
		return;

	if (_Idx == 0)
	{
		if (CameraChange)
			CameraChange(_Cam);
	}

	if (m_vecCam.size() <= _Idx + 1)
	{
		m_vecCam.resize(_Idx + 1);
	}

	// 동일한 우선순위의 카메라가 이미 존재하면 assert
	assert(nullptr == m_vecCam[_Idx]);

	m_vecCam[_Idx] = _Cam;
}

void CRenderMgr::RePositionDebugCam()
{
	CCamera* pMainCam = m_vecCam[0];

	Vec3 MainCamPos		 = m_vecCam[0]->Transform()->GetRelativePos();
	Vec3 MainCamFrontDir = m_vecCam[0]->Transform()->GetWorldDir(DIR_TYPE::FRONT);

	Vec3 MainCamRotation = m_vecCam[0]->Transform()->GetWorldRot();

	MainCamFrontDir.Normalize();

	MainCamFrontDir *= -100.f;
	MainCamPos += MainCamFrontDir;

	m_EditorCam->Transform()->SetRelativePos(MainCamPos);
	m_EditorCam->Transform()->SetRelativeRotation(MainCamRotation);
}

void CRenderMgr::CheckEscape()
{
	if (LEVEL_STATE::PLAY == CLevelMgr::GetInst()->GetCurrentLevel()->GetState())
	{
		if (KEY_TAP(F8) || KEY_TAP_EDITOR(F8))
		{
			m_bEscape = !m_bEscape;

			ActiveEditorMode(m_bEscape);
			static bool bPrevCursor = true;
			if (m_bEscape)
			{
				CKeyMgr::GetInst()->SetFocuseState(FOCUS_STATE::OTHER);
				bPrevCursor = CKeyMgr::GetInst()->IsShowCursor();
				CKeyMgr::GetInst()->RoRShowCursor(true);
				ShowCursor(true);
			}
			else
			{
				CKeyMgr::GetInst()->SetFocuseState(FOCUS_STATE::MAIN);
				CKeyMgr::GetInst()->RoRShowCursor(bPrevCursor);
				ShowCursor(bPrevCursor);
			}

			RePositionDebugCam();
		}
	}
}

CCamera* CRenderMgr::GetMainCam()
{
	if (LEVEL_STATE::PLAY == CLevelMgr::GetInst()->GetCurrentLevel()->GetState() && !IsEscape())
	{
		if (m_vecCam.empty())
			return nullptr;

		return m_vecCam[0];
	}
	else
	{
		return m_EditorCam;
	}
}
