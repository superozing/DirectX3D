#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CStructuredBuffer.h"

#include "CTimeMgr.h"
#include "CDevice.h"
#include "CAssetMgr.h"
#include "components.h"

#include "CMRT.h"

CRenderMgr::CRenderMgr()
	: m_arrMRT{}
	, m_Light2DBuffer(nullptr)
	, m_pDebugObj(nullptr)
	, m_DebugPosition(true)
	, m_EditorCam(nullptr)
	, m_RenderFunc(nullptr)
	, m_vClearColor(Vec4(0.f, 0.f, 0.f, 1.f))
{
	m_RenderFunc = &CRenderMgr::render_play;
}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_pDebugObj)
		delete m_pDebugObj;

	if (nullptr != m_Light2DBuffer)
		delete m_Light2DBuffer;

	if (nullptr != m_Light3DBuffer)
		delete m_Light3DBuffer;

	Delete_Array(m_arrMRT);
}

void CRenderMgr::tick()
{
	// 렌더타겟 및 깊이 타겟 설정
	ClearMRT();
	UpdateData();


	(this->*m_RenderFunc)();
	render_debug();

	Clear();
}

void CRenderMgr::ClearMRT()
{
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Clear();
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->ClearRT();
	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->ClearRT();
}

void CRenderMgr::render_play()
{
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->SortObject();
		m_vecCam[i]->render();
	}
}

void CRenderMgr::render_editor()
{
	if (nullptr == m_EditorCam)
		return;

	m_EditorCam->SortObject();
	m_EditorCam->render();
}

void CRenderMgr::render_debug()
{
	//레벨이 플레이상태일경우(랜더Func가 render_play 일 경우)
	if (&CRenderMgr::render_play == m_RenderFunc)
	{
		if (m_vecCam.empty())
			return;

		g_Transform.matView = m_vecCam[0]->GetViewMat();
		g_Transform.matProj = m_vecCam[0]->GetProjMat();
	}
	//레벨이 플레이상태가 아닐경우
	else
	{
		if (m_EditorCam == nullptr)
			return;

		g_Transform.matView = m_EditorCam->GetViewMat();
		g_Transform.matProj = m_EditorCam->GetProjMat();
	}

	list<tDebugShapeInfo>::iterator iter = m_DbgShapeInfo.begin();
	for (; iter != m_DbgShapeInfo.end(); )
	{
		switch ((*iter).eShape)
		{
		case DEBUG_SHAPE::RECT:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::CROSS:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CrossMesh"));
			break;

		case DEBUG_SHAPE::CUBE:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh_Debug"));
			break;
		case DEBUG_SHAPE::SPHERE:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
			break;
		case DEBUG_SHAPE::CONE:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"ConeMesh"));
		default:
			break;
		}

		m_pDebugObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));
		m_pDebugObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, (*iter).vColor);


		// 깊이판정 옵션 설정
		if ((*iter).bDepthTest)
			m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_WRITE);
		else
			m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);


		// 이전 Topology 저장
		D3D11_PRIMITIVE_TOPOLOGY PrevTopology = m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->GetTopology();
		if (DEBUG_SHAPE::CROSS == (*iter).eShape)
		{
			m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		}
		else if (DEBUG_SHAPE::SPHERE == (*iter).eShape)
		{
			m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_pDebugObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1); // Sphere Mesh임을 쉐이더 코드에 알림
		}
		else if (DEBUG_SHAPE::CONE == (*iter).eShape)
		{
			m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_pDebugObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 2); // Cone Mesh임을 쉐이더 코드에 알림
		}
		else
		{
			m_pDebugObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
		}

		m_pDebugObj->Transform()->SetWorldMat((*iter).matWorld);
		m_pDebugObj->Transform()->UpdateData();

		m_pDebugObj->render();

		m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(PrevTopology);

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

	if (m_vecCam.size() <= _Idx + 1)
	{
		m_vecCam.resize(_Idx + 1);
	}

	// 동일한 우선순위의 카메라가 이미 존재하면 assert
	assert(nullptr == m_vecCam[_Idx]);

	m_vecCam[_Idx] = _Cam;
}