#include "pch.h"
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

#include <Engine/CAssetMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CFSM.h>

#include "CIdleState.h"
#include "CTraceState.h"
#include <Scripts/CRenderMgrScript.h>


void CCreateTempLevel::Init()
{

	// 임시 FSM 객체 에셋 하나 생성하기
	Ptr<CFSM>	pFSM = new CFSM(true);

	pFSM->AddState(L"IdleState", new CIdleState);
	pFSM->AddState(L"TraceState", new CTraceState);

	CAssetMgr::GetInst()->AddAsset<CFSM>(L"NormalMonsterFSM", pFSM.Get());
}

void CCreateTempLevel::CreateTempLevel()
{

	CLevel* pTempLevel = new CLevel;


	// Main Camera Object 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraEffect);

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

	//pObj = pObj->Clone();
	//pObj->SetName(L"Light3D_Clone2");
	//pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	//pObj->Light3D()->SetLightColor(Vec3(0.3f, 1.f, 0.3f));
	//pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_DEFAULT, false);

	// SkyBox 용 오브젝트 추가
	pObj = new CGameObject;
	pObj->SetName(L"SkyBox");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CSkyBox);

	pObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));

	pObj->SkyBox()->SetSkyBoxType(SKYBOX_TYPE::SPHERE);
	pObj->SkyBox()->SetSphereTexture(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\Sky02.jpg", L"texture\\skybox\\Sky02.jpg"));

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_DEFAULT, false);

	// Player Object 생성
	pObj = new CGameObject;
	pObj->SetName(L"Sphere");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
	//pObj->Transform()->SetDir(Vec3(0.f, -1.f, 0.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHsphere));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std3D_DeferredMtrl"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01.tga", L"texture\\tile\\TILE_01.tga"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01_N.tga", L"texture\\tile\\TILE_01_N.tga"));

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_PLAYER, false);


	// LandScape Object 생성
	CGameObject* LandScape = new CGameObject;
	LandScape->SetName(L"LandScape");

	LandScape->AddComponent(new CTransform);
	LandScape->AddComponent(new CLandScape);

	LandScape->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	LandScape->Transform()->SetRelativeScale(Vec3(200.f, 1000.f, 200.f));

	LandScape->LandScape()->SetHeightMap(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\HeightMap\\HeightMap_01.jpg"));

	pTempLevel->AddObject(LandScape, (UINT)LAYER::LAYER_DEFAULT, false);


	// TimeMgr Object 생성
	pObj = new CGameObject;
	pObj->SetName(L"Manager Object");

	pObj->AddComponent(new CTimeMgrScript);
	pObj->AddComponent(new CRenderMgrScript);

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_DEFAULT, false);


	// 충돌 설정
	CCollisionMgr::GetInst()->LayerCheck((UINT)LAYER::LAYER_PLAYER, (UINT)LAYER::LAYER_MONSTER);
	CCollisionMgr::GetInst()->LayerCheck((UINT)LAYER::LAYER_MONSTER, (UINT)LAYER::LAYER_MONSTER);

	CLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::STOP);

	CLevelSaveLoad::SaveLevel(pTempLevel, L"level\\temp.lv");

	CTaskMgr::GetInst()->tick();

}