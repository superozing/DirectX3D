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

#include <Engine/CAssetMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CFSM.h>

#include "CIdleState.h"
#include "CTraceState.h"
#include <Scripts/CRenderMgrScript.h>


void CCreateTempLevel::Init()
{
	// Missile Prefab 생성
	/*CGameObject* pObj = nullptr;

	pObj = new CGameObject;
	pObj->SetName(L"Missile");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CMissileScript);

	pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	Ptr<CPrefab> pMissilePrefab = new CPrefab(pObj);
	CAssetMgr::GetInst()->AddAsset<CPrefab>(L"MissilePrefab", pMissilePrefab.Get());


	//pMissilePrefab->Save(L"prefab\\missile.pref");
	*/

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

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	//pObj->Transform()->SetRelativeRotation(Vec3(XM_PI / 4.f, XM_PI / 4.f, 0.f));

	pObj->Light3D()->SetLightType(LIGHT_TYPE::POINT);
	pObj->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pObj->Light3D()->SetAmbient(Vec3(0.0f, 0.0f, 0.0f));
	pObj->Light3D()->SetSpecular(Vec3(0.2f, 0.2f, 0.2f));
	pObj->Light3D()->SetRadius(500.f);

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_DEFAULT, false);

	pObj = pObj->Clone();
	pObj->SetName(L"Light3D_Clone2");
	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pObj->Light3D()->SetLightColor(Vec3(0.3f, 1.f, 0.3f));
	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_DEFAULT, false);


	// SkyBox 용 오브젝트 추가
	pObj = new CGameObject;
	pObj->SetName(L"SkyBox");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CSkyBox);

	pObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));

	pObj->SkyBox()->SetSkyBoxType(SKYBOX_TYPE::SPHERE);
	pObj->SkyBox()->SetSphereTexture(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\Sky02.jpg", L"texture\\skybox\\Sky02.jpg"));

	//pObj->SkyBox()->SetSkyBoxType(SKYBOX_TYPE::CUBE);
	//pObj->SkyBox()->SetCubeTexture(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\SkyWater.dds", L"texture\\skybox\\SkyWater.dds"));

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_DEFAULT, false);


	// Player Object 생성
	pObj = new CGameObject;
	pObj->SetName(L"Player");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CAnimator2D);
	pObj->AddComponent(new CPlayerScript);
	pObj->AddComponent(new CMissileScript);


	//Ptr<CTexture> pAltasTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\link.png", L"texture\\link.png");
	//pObj->Animator2D()->Create(L"IDLE_UP", pAltasTex, Vec2(0.f, 260.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 1, 10);
	//pObj->Animator2D()->Create(L"IDLE_DOWN", pAltasTex, Vec2(0.f, 0.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 3, 10);
	//wstring str = CPathMgr::GetContentPath();
	//str += L"anim\\";
	//pObj->Animator2D()->SaveAllAnim(str);

	pObj->Transform()->SetRelativePos(Vec3(0.f, -200.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(2000.f, 2000.f, 1.f));
	pObj->Transform()->SetRelativeRotation(Vec3(XM_PI / 2.f, 0.f, 0.f));

	pObj->Collider2D()->SetAbsolute(true);
	pObj->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std3D_DeferredMtrl"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01.tga", L"texture\\tile\\TILE_01.tga"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01_N.tga", L"texture\\tile\\TILE_01_N.tga"));

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_PLAYER, false);


	// TimeMgr Object 생성
	pObj = new CGameObject;
	pObj->SetName(L"Manager Object");

	pObj->AddComponent(new CTimeMgrScript);
	pObj->AddComponent(new CRenderMgrScript);

	pTempLevel->AddObject(pObj, 0);


	// 충돌 설정
	CCollisionMgr::GetInst()->LayerCheck((UINT)LAYER::LAYER_PLAYER, (UINT)LAYER::LAYER_MONSTER);
	CCollisionMgr::GetInst()->LayerCheck((UINT)LAYER::LAYER_MONSTER, (UINT)LAYER::LAYER_MONSTER);

	CLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::STOP);

	CLevelSaveLoad::SaveLevel(pTempLevel, L"level\\temp.lv");

	CTaskMgr::GetInst()->tick();

}