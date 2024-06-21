#include "pch.h"
#include "CCreatePlayerTestLevel.h"

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
#include "CCreatePlayerTestLevel.h"

void CCreatePlayerTestLevel::CreateTempLevel()
{
	CLevel *pTempLevel = new CLevel;
	pTempLevel->SetName("PlayerTestLevel");

	// Main Camera Object 생성
	CGameObject *pCamObj = new CGameObject;
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

	CGameObject *pObj = nullptr;

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

	pObj->SkyBox()->SetSkyBoxType(SKYBOX_TYPE::CUBE);
	pObj->SkyBox()->SetCubeTexture(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\SkyDawn.dds"));

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_DEFAULT, false);

	// Player Object 생성
	pObj = new CGameObject;
	pObj->SetName(L"Plain");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(0.f, -500.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(5000.f, 5000.f, 500.f));
	pObj->Transform()->SetRelativeRotation(Vec3(XM_PI / 2.f, 0.f, 0.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));

	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std3D_DeferredMtrl"), 0);
	pObj->MeshRender()->GetMaterial(0)->SetTexParam(
	    TEX_PARAM::TEX_0,
	    CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01.tga", L"texture\\tile\\TILE_01.tga"));
	pObj->MeshRender()->GetMaterial(0)->SetTexParam(
	    TEX_PARAM::TEX_1,
	    CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01_N.tga", L"texture\\tile\\TILE_01_N.tga"));

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_PLAYER, false);

	// ============
	// FBX Loading
	// ============
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject *pObj = nullptr;

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\serika_better2.fbx");
		//pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\serika_Merge.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(pMeshData->GetKey());
		pObj->Transform()->SetRelativePos(Vec3(-100.f, -40.f, 100.f));
		pObj->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 300.f));
		pObj->Transform()->SetRelativeRotation(Vec3(0.f, -XM_PI / 2.f, 0.f));

		pTempLevel->AddObject(pObj, 0, false);
	}

	//{
	//	Ptr<CMeshData> pMeshData = nullptr;
	//	CGameObject* pObj = nullptr;

	//	pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\serika_Merge.fbx");
	//	pObj = pMeshData->Instantiate();
	//	pObj->SetName(pMeshData->GetKey());
	//	pObj->Transform()->SetRelativePos(Vec3(200.f, -40.f, 100.f));
	//	pObj->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 300.f));
	//	pObj->Transform()->SetRelativeRotation(Vec3(0.f, -XM_PI / 2.f, 0.f));

	//	pTempLevel->AddObject(pObj, 0, false);
	//}

	GamePlayStatic::ChangeLevel(pTempLevel, LEVEL_STATE::STOP);

	CLevelSaveLoad::SaveLevel(pTempLevel, L"level\\PlayerTest.lv");
}
