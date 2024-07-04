﻿#include "pch.h"
#include "CCreateTempLevel.h"

#include <Engine/CCollisionMgr.h>

#include <Engine/CTaskMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Scripts/CPlayerScript.h>
#include <Scripts/CBackgroundScript.h>

#include <Engine/CMesh.h>
#include <Engine/CGraphicsShader.h>
#include <Engine/CTexture.h>
#include <Engine/CSetColorShader.h>

#include "CLevelSaveLoad.h"
#include <Scripts/CMissileScript.h>
#include <Scripts/CMonsterScript.h>

#include <Scripts/CTimeMgrScript.h>
#include <Scripts/CCameraEffect.h>
#include <Scripts/CCameraMoveScript.h>

#include <Engine/CAssetMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CFSM.h>

#include "CIdleState.h"
#include "CTraceState.h"
#include <Scripts/CRenderMgrScript.h>
#include <Engine/CPhysX.h>
#include <Scripts\CPhysXMgrScript.h>

void CCreateTempLevel::Init()
{

	// 임시 FSM 객체 에셋 하나 생성하기
	Ptr<CFSM> pFSM = new CFSM(true);

	pFSM->AddState(L"IdleState", new CIdleState);
	pFSM->AddState(L"TraceState", new CTraceState);

	CAssetMgr::GetInst()->AddAsset<CFSM>(L"NormalMonsterFSM", pFSM.Get());
}

void CCreateTempLevel::CreateTempLevel()
{

	CLevel* pTempLevel = new CLevel;
	pTempLevel->SetName("TempLevel");

#pragma region About Camera
	// Main Camera Object 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraEffect);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pCamObj->Camera()->SetCameraPriority(0);
	pCamObj->Camera()->LayerCheckAll();
	pCamObj->Camera()->LayerCheck(31, false);

	pTempLevel->AddObject(pCamObj, (UINT)LAYER::LAYER_DEFAULT);

	// UI 카메라 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"UICamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(1);
	pCamObj->Camera()->LayerCheck(31, true);

	pTempLevel->AddObject(pCamObj, (UINT)LAYER::LAYER_DEFAULT);
	CGameObject* pObj = nullptr;
#pragma endregion

#pragma region About Light
	// 3D Light 추가
	pObj = new CGameObject;
	pObj->SetName(L"Light3D");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CLight3D);

	pObj->Transform()->SetRelativePos(Vec3(-1000.f, 2000.f, -1000.f));
	pObj->Transform()->SetDir(Vec3(1.f, -1.f, 1.f));

	pObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pObj->Light3D()->SetLightColor(Vec3(0.8f, 0.8f, 0.8f));
	pObj->Light3D()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
	pObj->Light3D()->SetSpecular(Vec3(0.05f, 0.05f, 0.05f));
	pObj->Light3D()->SetRadius(500.f);
	pObj->Light3D()->SetAngle(1.f);

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_DEFAULT, false);
#pragma endregion

#pragma region About SkyBox
	// SkyBox 용 오브젝트 추가
	pObj = new CGameObject;
	pObj->SetName(L"SkyBox");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CSkyBox);

	pObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));

	pObj->SkyBox()->SetSkyBoxType(SKYBOX_TYPE::SPHERE);
	pObj->SkyBox()->SetSphereTexture(
		CAssetMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\Sky02.jpg", L"texture\\skybox\\Sky02.jpg"));

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_DEFAULT, false);
#pragma endregion

	// Sphere Object 생성
	pObj = new CGameObject;
	pObj->SetName(L"Sphere");

	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
	// pObj->Transform()->SetDir(Vec3(0.f, -1.f, 0.f));

	pObj->AddComponent(new CMeshRender);
	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHsphere));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std3D_DeferredMtrl"), 0);
	pObj->MeshRender()->GetMaterial(0)->SetTexParam(
		TEX_PARAM::TEX_0,
		CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01.tga", L"texture\\tile\\TILE_01.tga"));
	pObj->MeshRender()->GetMaterial(0)->SetTexParam(
		TEX_PARAM::TEX_1,
		CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01_N.tga", L"texture\\tile\\TILE_01_N.tga"));

	pObj->AddComponent(new CPhysX);
	pObj->PhysX()->m_bStaticActor = false;
	pObj->PhysX()->m_Shape		  = PhysShape::SPHERE;

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_PLAYER, false);
	pObj = nullptr;

	// Sphere Object2 생성
	// pObj = new CGameObject;
	// pObj->SetName(L"Sphere2");
	//
	// pObj->AddComponent(new CTransform);
	// pObj->Transform()->SetRelativePos(Vec3(0.f, 600.f, 500.f));
	// pObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
	//
	// pObj->AddComponent(new CMeshRender);
	// pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHsphere));
	// pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std3D_DeferredMtrl"), 0);
	// pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0,
	// CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01.tga", L"texture\\tile\\TILE_01.tga"));
	// pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_1,
	// CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01_N.tga", L"texture\\tile\\TILE_01_N.tga"));
	//
	// pObj->AddComponent(new CPhysX);
	// pObj->PhysX()->m_bStaticActor = false;
	//
	// pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_PLAYER, false);
	// pObj = nullptr;

	// Floor Object 생성
	pObj = new CGameObject;
	pObj->SetName(L"Floor");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(0.f, -500.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(500.f, 2000.f, 200.f));
	pObj->Transform()->SetDir(Vec3(0.f, -1.f, 0.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));

	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std3D_DeferredMtrl"), 0);
	pObj->MeshRender()->GetMaterial(0)->SetTexParam(
		TEX_PARAM::TEX_0,
		CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01.tga", L"texture\\tile\\TILE_01.tga"));
	pObj->MeshRender()->GetMaterial(0)->SetTexParam(
		TEX_PARAM::TEX_1,
		CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01_N.tga", L"texture\\tile\\TILE_01_N.tga"));

	pObj->AddComponent(new CPhysX);
	pObj->PhysX()->m_bStaticActor = true;
	// pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_TILE, false);
	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_MONSTER, false);

#pragma region About LandScape
	//// LandScape Object 생성
	// CGameObject* LandScape = new CGameObject;
	// LandScape->SetName(L"LandScape");
	//
	// LandScape->AddComponent(new CTransform);
	// LandScape->AddComponent(new CLandScape);
	//
	// LandScape->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	// LandScape->Transform()->SetRelativeScale(Vec3(200.f, 1000.f, 200.f));
	//
	////LandScape->LandScape()->SetHeightMap(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\HeightMap\\HeightMap_01.jpg"));
	// LandScape->LandScape()->SetHeightMap(CAssetMgr::GetInst()->FindAsset<CTexture>(L"HeightMapTex"));

	// Particle Object
	// pObj = new CGameObject;
	// pObj->SetName(L"Particle Test");

	// pObj->AddComponent(new CTransform);
	// pObj->AddComponent(new CParticleSystem);

	// pObj->Transform()->SetRelativePos(Vec3(0.f, 200.f, 500.f));
	// pObj->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 20.f));
	// pObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	// pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_DEFAULT, false);
#pragma endregion

#pragma region About Mgr
	// TimeMgr Object 생성
	pObj = new CGameObject;
	pObj->SetName(L"Manager Object");

	pObj->AddComponent(new CTimeMgrScript);
	pObj->AddComponent(new CRenderMgrScript);
	pObj->AddComponent(new CPhysXMgrScript);

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_DEFAULT, false);
#pragma endregion

#pragma region About FBX
	// ============
	// FBX Loading
	// ============
	//{
	//	Ptr<CMeshData> pMeshData = nullptr;
	//	CGameObject* pObj = nullptr;

	//	pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\house.fbx");
	//	//pMeshData = CAssetMgr::GetInst()->FindAsset<CMeshData>(L"meshdata\\house.mdat");
	//	pObj = pMeshData->Instantiate();
	//	pObj->SetName(L"House");

	//	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));

	//	pTempLevel->AddObject(pObj, 0, false);
	//}
#pragma endregion

#pragma region About Collision
	// 충돌 설정
	CCollisionMgr::GetInst()->LayerCheck((UINT)LAYER::LAYER_PLAYER, (UINT)LAYER::LAYER_MONSTER);
	CCollisionMgr::GetInst()->LayerCheck((UINT)LAYER::LAYER_MONSTER, (UINT)LAYER::LAYER_MONSTER);
#pragma endregion

	GamePlayStatic::ChangeLevel(pTempLevel, LEVEL_STATE::STOP);
	CLevelSaveLoad::SaveLevel(pTempLevel, L"level\\temp.lv");
}