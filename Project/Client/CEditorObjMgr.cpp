#include "pch.h"
#include "CEditorObjMgr.h"

#include "CGameObjectEx.h"
#include <Engine/components.h>
#include <Scripts/CCameraMoveScript.h>

#include <Engine/CRenderMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include <Scripts/CCameraEffect.h>
#include <Scripts\CEditorCameraMoveScript.h>

CEditorObjMgr::CEditorObjMgr()
{
}

CEditorObjMgr::~CEditorObjMgr()
{
	Delete_Vec(m_vecEditorObj);
}

void CEditorObjMgr::init()
{
	// 에디터용 카메라 오브젝트 생성
	m_EditorCam = new CGameObjectEx;
	m_EditorCam->SetName(L"Editor Camera");
	m_EditorCam->AddComponent(new CTransform);
	m_EditorCam->AddComponent(new CCamera);
	m_EditorCam->AddComponent(new CEditorCameraMoveScript);
	m_EditorCam->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	m_EditorCam->Transform()->SetRelativeRotation(Vec3(.0f, 0.f, 0.f));

	m_EditorCam->Camera()->LayerCheckAll();
	m_EditorCam->Camera()->LayerCheck(31, false);
	m_EditorCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	m_EditorCam->Camera()->SetFOV(XM_PI / 2.f);
	m_EditorCam->Camera()->SetFar(100000.f);

	auto pComp = new CCameraEffect;
	m_EditorCam->AddComponent(pComp);
	pComp->begin();
	m_vecEditorObj.push_back(m_EditorCam);

	// Editor 용 카메라로서 렌더매니저에 등록
	CRenderMgr::GetInst()->RegisterEditorCamera(m_EditorCam->Camera());
}

void CEditorObjMgr::progress()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->tick();
	}

	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->finaltick();
	}
}
