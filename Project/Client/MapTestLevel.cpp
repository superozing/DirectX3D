#include "pch.h"
#include "MapTestLevel.h"

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

void MapTestLevel::Init()
{
}

void MapTestLevel::CreateMapTestLevel()
{
	CLevel* pTempLevel = new CLevel;
	pTempLevel->SetName("MapLevel");

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

	// ============
	// FBX  land Loading
	// ============
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject*   pObj		 = nullptr;

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\Map_Grass.fbx");
		// pMeshData = CAssetMgr::GetInst()->FindAsset<CMeshData>(L"meshdata\\house.mdat");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"grass");

		pObj->Transform()->SetRelativePos(Vec3(13520.f, 2855.f, 3490.f));
		pObj->Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(1.5f, 1.5f, 1.75f));

		pTempLevel->AddObject(pObj, 0, false);

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\Map_Hills.fbx");
		pObj	  = pMeshData->Instantiate();
		pObj->SetName(L"hill");

		pObj->Transform()->SetRelativePos(Vec3(1445.f, 2860.f, 10560.f));

		pTempLevel->AddObject(pObj, 0, false);

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\Map_Parcel.fbx");

		pObj = pMeshData->Instantiate();
		pObj->SetName(L"parcel");

		pObj->Transform()->SetRelativePos(Vec3(8730.f, 2862.f, 3484.f));

		pTempLevel->AddObject(pObj, 0, false);

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\Map_DirtEdge.fbx");
		pObj	  = pMeshData->Instantiate();
		pObj->SetName(L"dirtedge");

		pObj->Transform()->SetRelativePos(Vec3(8820.f, 2862.f, 3505.f));

		pTempLevel->AddObject(pObj, 0, false);

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\Map_Sidewalk.fbx");
		pObj	  = pMeshData->Instantiate();
		pObj->SetName(L"SideWalk");

		pObj->Transform()->SetRelativePos(Vec3(10515.f, 2890.f, -3415.f));
		pObj->Transform()->SetRelativeRotation(Vec3(0.f, -XM_PI / 2.f, 0.f));

		pTempLevel->AddObject(pObj, 0, false);

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\Map_Road.fbx");
		pObj	  = pMeshData->Instantiate();
		pObj->SetName(L"Road");

		pObj->Transform()->SetRelativePos(Vec3(12050.f, 2885.f, -1185.f));
		pObj->Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));

		pTempLevel->AddObject(pObj, 0, false);
	}

	// =====================
	// FBX Map Object Loading
	// =====================
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject*   pObj		 = nullptr;

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\House_A.fbx");
		pObj	  = pMeshData->Instantiate();
		pObj->SetName(L"HouseA");

		pObj->Transform()->SetRelativePos(Vec3(-5103.f, 3068.f, -10036.f));
		pObj->Transform()->SetRelativeRotation(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

		pTempLevel->AddObject(pObj, 0, false);

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\House_B.fbx");
		pObj	  = pMeshData->Instantiate();
		pObj->SetName(L"HouseB");

		pObj->Transform()->SetRelativePos(Vec3(-7150.f, 3068.f, -10230.f));
		pObj->Transform()->SetRelativeRotation(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

		pTempLevel->AddObject(pObj, 0, false);

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\House_C.fbx");
		pObj	  = pMeshData->Instantiate();
		pObj->SetName(L"HouseC");

		pObj->Transform()->SetRelativePos(Vec3(-9465.f, 3068.f, -10230.f));
		pObj->Transform()->SetRelativeRotation(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

		pTempLevel->AddObject(pObj, 0, false);

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\Tower_A.fbx");
		pObj	  = pMeshData->Instantiate();
		pObj->SetName(L"TowerA");

		pObj->Transform()->SetRelativePos(Vec3(-5245.f, 3068.f, -7725.f));
		pObj->Transform()->SetRelativeRotation(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

		pTempLevel->AddObject(pObj, 0, false);

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\Tower_B.fbx");
		pObj	  = pMeshData->Instantiate();
		pObj->SetName(L"TowerB");

		pObj->Transform()->SetRelativePos(Vec3(-7150.f, 3068.f, -7785.f));
		pObj->Transform()->SetRelativeRotation(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

		pTempLevel->AddObject(pObj, 0, false);

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\Tower_C.fbx");
		pObj	  = pMeshData->Instantiate();
		pObj->SetName(L"TowerC");

		pObj->Transform()->SetRelativePos(Vec3(-9310.f, 3068.f, -7695.f));
		pObj->Transform()->SetRelativeRotation(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

		pTempLevel->AddObject(pObj, 0, false);

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\Tower_D.fbx");
		pObj	  = pMeshData->Instantiate();
		pObj->SetName(L"TowerD");

		pObj->Transform()->SetRelativePos(Vec3(-8985.f, 3068.f, -5625.f));
		pObj->Transform()->SetRelativeRotation(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

		pTempLevel->AddObject(pObj, 0, false);

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\House_D.fbx");
		pObj	  = pMeshData->Instantiate();
		pObj->SetName(L"houseD");

		pObj->Transform()->SetRelativePos(Vec3(-9515.f, 2995.f, -4150.f));
		pObj->Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 150.f));

		pTempLevel->AddObject(pObj, 0, false);

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\House_E.fbx");
		pObj	  = pMeshData->Instantiate();
		pObj->SetName(L"houseE");

		pObj->Transform()->SetRelativePos(Vec3(-5455.f, 2895.f, -615.f));
		pObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
		pObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

		pTempLevel->AddObject(pObj, 0, false);

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\Tent.fbx");
		pObj	  = pMeshData->Instantiate();
		pObj->SetName(L"Tent");

		pObj->Transform()->SetRelativePos(Vec3(-3600.f, 2885.f, -6160.f));
		pObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
		pObj->Transform()->SetRelativeRotation(Vec3(-XM_PI / 2.f, 0.f, 0.f));

		pTempLevel->AddObject(pObj, 0, false);
	}

	GamePlayStatic::ChangeLevel(pTempLevel, LEVEL_STATE::STOP);

	// CLevelSaveLoad::SaveLevel(pTempLevel, L"level\\MapLevel.lv");

	// CTaskMgr::GetInst()->tick();
}
