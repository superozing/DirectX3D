﻿#include "pch.h"
#include "CEditorObjMgr.h"

#include "CGameObjectEx.h"
#include <Engine/components.h>
#include <Scripts/CCameraMoveScript.h>

#include <Engine/CRenderMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include <Scripts/CCameraEffect.h>
#include <Scripts\CEditorCameraMoveScript.h>
#include <Engine\CMemoryPoolMgr.h>
#include <Scripts\CMemoryPoolMgrScript.h>

CEditorObjMgr::CEditorObjMgr()
{
}

CEditorObjMgr::~CEditorObjMgr()
{
	Delete_Array(m_arrEditorObj);
}

#include <Scripts/CPhysXMgrScript.h>
#include <Scripts/CRenderMgrScript.h>
#include <Scripts/CTimeMgrScript.h>
void CEditorObjMgr::init()
{
	// 에디터용 카메라 오브젝트 생성
	if (!m_arrEditorObj[(UINT)EDITOROBJ_TYPE::EditorCam])
	{
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
		// pComp->begin();
		m_arrEditorObj[(UINT)EDITOROBJ_TYPE::EditorCam] = m_EditorCam;

		// Editor 용 카메라로서 렌더매니저에 등록
		CRenderMgr::GetInst()->RegisterEditorCamera(m_EditorCam->Camera());
	}

	// 매니저 스크립트 오브젝트 생성
	m_ManagerObj = new CGameObjectEx;
	m_ManagerObj->SetName(L"Manager Object");
	auto PS = new CPhysXMgrScript;
	// PS->begin();
	m_ManagerObj->AddComponent(PS);
	auto RS = new CRenderMgrScript;
	// RS->begin();
	m_ManagerObj->AddComponent(RS);
	auto TS = new CTimeMgrScript;
	// TS->begin();
	m_ManagerObj->AddComponent(TS);
	m_arrEditorObj[(UINT)EDITOROBJ_TYPE::Manager] = m_ManagerObj;

	m_MemoryPool = new CGameObjectEx;
	m_MemoryPool->SetName(L"Memory Pool");
	m_MemoryPool->AddComponent(new CTransform);

	m_MemoryPool->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	m_MemoryPool->Transform()->SetRelativeRotation(Vec3(.0f, 0.f, 0.f));
	auto PoolScript = new CMemoryPoolMgrScript;
	m_MemoryPool->AddComponent(PoolScript);
	m_arrEditorObj[(UINT)EDITOROBJ_TYPE::MemoryPool] = m_MemoryPool;

	CGameObject* pObj = dynamic_cast<CGameObject*>(m_MemoryPool);
	CMemoryPoolMgr::GetInst()->SetEX(pObj);

	CEditorObjMgr::begin();
}

void CEditorObjMgr::begin()
{
	for (size_t i = 0; i < (UINT)EDITOROBJ_TYPE::END; ++i)
	{
		m_arrEditorObj[i]->begin();
	}
}

void CEditorObjMgr::progress()
{
	for (size_t i = 0; i < (UINT)EDITOROBJ_TYPE::END; ++i)
	{
		m_arrEditorObj[i]->tick();
	}

	for (size_t i = 0; i < (UINT)EDITOROBJ_TYPE::END; ++i)
	{
		m_arrEditorObj[i]->finaltick();
	}
}

void CEditorObjMgr::SetEditorCam(CGameObject* _obj)
{
	if (m_EditorCam)
		delete m_EditorCam;
	m_EditorCam										= (CGameObjectEx*)_obj;
	m_arrEditorObj[(UINT)EDITOROBJ_TYPE::EditorCam] = m_EditorCam;
	CRenderMgr::GetInst()->RegisterEditorCamera(m_EditorCam->Camera());
}
