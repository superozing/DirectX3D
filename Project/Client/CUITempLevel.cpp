#include "pch.h"
#include "CUITempLevel.h"

#include <Engine/CCollisionMgr.h>

#include <Engine/CTaskMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CMesh.h>
#include <Engine/CGraphicsShader.h>
#include <Engine/CTexture.h>
#include <Engine/CFontRender.h>

#include "CLevelSaveLoad.h"

#include <Scripts/CTimeMgrScript.h>
#include <Scripts/CCameraEffect.h>
#include <Scripts/CCameraMoveScript.h>

#include <Engine/CAssetMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CFSM.h>

#include <Scripts/CRenderMgrScript.h>

#include <Scripts/CBtnUIScript.h>
#include <Scripts/CpanelUIScript.h>
#include <Engine/CDevice.h>

#include <Scripts/CProgressBar.h>
#include <Scripts/CCrosshair.h>
#include <Scripts/CWeaponInfo.h>
#include <Scripts/CPausePanel.h>
#include <Engine/CFontMgr.h>

void CUITempLevel::Init()
{
}

void CUITempLevel::CreateTempLevel()
{

#pragma region default object

	CLevel* pTempLevel = new CLevel;
	pTempLevel->SetName("TempLevel");

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

	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pCamObj->Camera()->SetCameraPriority(1);
	pCamObj->Camera()->LayerCheck((UINT)LAYER::LAYER_UI, true);

	pCamObj->Camera()->SetFar(10000.f);
	pCamObj->Camera()->SetScale(1.f);

	auto res = CDevice::GetInst()->GetRenderResolution();

	pCamObj->Camera()->SetWidth(res.x);
	pCamObj->Camera()->SetAspectRatio(res.x / res.y);

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

	// Sphere Object 생성
	pObj = new CGameObject;
	pObj->SetName(L"Sphere");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
	// pObj->Transform()->SetDir(Vec3(0.f, -1.f, 0.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHsphere));

	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std3D_DeferredMtrl"), 0);
	pObj->MeshRender()->GetMaterial(0)->SetTexParam(
		TEX_PARAM::TEX_0,
		CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01.tga", L"texture\\tile\\TILE_01.tga"));
	pObj->MeshRender()->GetMaterial(0)->SetTexParam(
		TEX_PARAM::TEX_1,
		CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01_N.tga", L"texture\\tile\\TILE_01_N.tga"));

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_PLAYER, false);

#pragma endregion

#pragma region how to add UI Script

	// auto pBtnTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Background.jpg", L"texture\\Background.jpg");
	//
	//
	//// DynamicUI Object 생성
	//// MESHpoint
	//// DynamicUIMtrl
	//// btnUI2->DisableMouseInput(); -> 카메라의 영향을 받는 UI는 이걸 반드시 꺼주어야 해요.
	//
	// pObj = new CGameObject;
	// pObj->SetName(L"DynamicUI");

	// pObj->AddComponent(new CTransform);
	// pObj->AddComponent(new CMeshRender);

	// auto btnUI2 = new CBtnUIScript;
	// pObj->AddComponent(btnUI2);

	// btnUI2->DisableMouseInput();

	// btnUI2->AllowTexSet();
	// btnUI2->SetNormalImg(pBtnTex);

	// pObj->Transform()->SetRelativePos(Vec3(0, 0, 100.f));
	// pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	// pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHpoint)); // pointMesh일 때, 동작하지 않음.
	// 무엇의 문제인지는 모르겠네요.
	// pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DynamicUIMtrl"), 0);
	// pObj->MeshRender()->GetDynamicMaterial(0);
	// pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pBtnTex);

	// pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_DEFAULT, false);

	//// PanelUI Object 생성
	//// MESHrect
	//// StaticUIMtrl
	// auto Panel1 = new CGameObject;
	// Panel1->SetName(L"PanelUI");

	// Panel1->AddComponent(new CTransform);
	// Panel1->AddComponent(new CMeshRender);

	// auto PanelUI1 = new CPanelUIScript;
	// Panel1->AddComponent(PanelUI1);

	// PanelUI1->AllowTexSet();
	// PanelUI1->SetPanelTex(pBtnTex);
	// PanelUI1->AllowDragAndDrop();

	// Panel1->Transform()->SetRelativePos(Vec3(-500, 200, 100.f));
	// Panel1->Transform()->SetRelativeScale(Vec3(1000.f, 1000.f, 1.f));

	// Panel1->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect)); // pointMesh일 때, 동작하지
	// 않음. 무엇의 문제인지는 모르겠네요.
	// Panel1->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	// Panel1->MeshRender()->GetDynamicMaterial(0);
	// Panel1->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pBtnTex);

	// pTempLevel->AddObject(Panel1, (UINT)LAYER::LAYER_UI, false);

	//// StaticBtnUI Object 생성
	//// MESHrect
	//// StaticUIMtrl
	// pObj = new CGameObject;
	// pObj->SetName(L"StaticBtnUI");

	// pObj->AddComponent(new CTransform);
	// pObj->AddComponent(new CMeshRender);

	// auto btnUI = new CBtnUIScript;
	// pObj->AddComponent(btnUI);
	// btnUI->AllowTexSet();
	// btnUI->SetNormalImg(pBtnTex);

	// pObj->Transform()->SetRelativePos(Vec3(0, 0, 100.f));
	// pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	// pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	// pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	// pObj->MeshRender()->GetDynamicMaterial(0);
	// pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pBtnTex);

	// Panel1->AddChild(pObj);

	//// PanelUI2 Object 생성
	//// MESHrect
	//// StaticUIMtrl
	// pObj = new CGameObject;
	// pObj->SetName(L"PanelUI2");

	// pObj->AddComponent(new CTransform);
	// pObj->AddComponent(new CMeshRender);

	// auto PanelUI2 = new CPanelUIScript;
	// pObj->AddComponent(PanelUI2);

	// PanelUI2->AllowTexSet();
	// PanelUI2->SetPanelTex(pBtnTex);
	// PanelUI2->AllowDragAndDrop();

	// pObj->Transform()->SetRelativePos(Vec3(-500, 200, 100.f));
	// pObj->Transform()->SetRelativeScale(Vec3(1000.f, 1000.f, 1.f));

	// pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect)); // pointMesh일 때, 동작하지 않음.
	// 무엇의 문제인지는 모르겠네요.
	// pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	// pObj->MeshRender()->GetDynamicMaterial(0);
	// pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pBtnTex);

	// pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_UI, false);

#pragma endregion

#pragma region progress bar

	//// Progress Bar
	// auto BarObj		 = new CGameObject;
	// auto progressbar = new CProgressBar;

	// BarObj->SetName(L"Progress Bar");

	// BarObj->AddComponent(new CTransform);
	// BarObj->AddComponent(progressbar);

	// progressbar->SetMaxHP(2000);
	// progressbar->SetCurHP(2000);

	// progressbar->SetImgFontTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/ImageFont_Raidboss.png"));
	// progressbar->SetPortraitTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Portrait_Raidboss_KaitenRanger.png"));

	// BarObj->Transform()->SetRelativePos(Vec3(0.f, 300.f, 100.f));
	// BarObj->Transform()->SetRelativeScale(Vec3(1, 1, 1.f));

	// pTempLevel->AddObject(BarObj, (UINT)LAYER::LAYER_UI, false);

	//// Add HP Btn
	// pObj = new CGameObject;
	// pObj->SetName(L"Add HP Btn");

	// pObj->AddComponent(new CTransform);
	// pObj->AddComponent(new CMeshRender);

	// auto AddHPbtnUI = new CBtnUIScript;
	// pObj->AddComponent(AddHPbtnUI);
	// AddHPbtnUI->AllowTexSet();
	// AddHPbtnUI->SetNormalImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HP_Red.png"));
	// AddHPbtnUI->SetDeletage((CEntity*)progressbar, (DelegateFunc)&CProgressBar::Add100);

	// pObj->Transform()->SetRelativePos(Vec3(680, -400, 100.f));
	// pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	// pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	// pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	// pObj->MeshRender()->GetDynamicMaterial(0);
	// pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0,
	//												CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HP_Red.png"));

	// pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_UI, false);

	//// Sub HP Btn
	// pObj = new CGameObject;
	// pObj->SetName(L"Sub HP Btn");

	// pObj->AddComponent(new CTransform);
	// pObj->AddComponent(new CMeshRender);

	// auto SubHPbtnUI = new CBtnUIScript;
	// pObj->AddComponent(SubHPbtnUI);
	// SubHPbtnUI->AllowTexSet();
	// SubHPbtnUI->SetNormalImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HP_Yellow.png"));
	// SubHPbtnUI->SetDeletage((CEntity*)progressbar, (DelegateFunc)&CProgressBar::Sub100);

	// pObj->Transform()->SetRelativePos(Vec3(800, -400, 100.f));
	// pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	// pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	// pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	// pObj->MeshRender()->GetDynamicMaterial(0);
	// pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0,
	//												CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HP_Yellow.png"));

	// pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_UI, false);

#pragma endregion

	// CWeaponInfo
	pObj = new CGameObject;
	pObj->SetName(L"WeaponInfo");

	pObj->AddComponent(new CTransform);

	auto pWeaponInfo = new CWeaponInfo;
	pObj->AddComponent(pWeaponInfo);
	pWeaponInfo->SetMaxAmmo(30);
	pWeaponInfo->SetCurAmmo(30);

	pObj->Transform()->SetRelativePos(Vec3(700, -300, 0));
	pObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_UI, false);

	// CCrosshair
	pObj = new CGameObject;
	pObj->SetName(L"Crosshair");

	pObj->AddComponent(new CTransform);

	auto pCrosshair = new CCrosshair;
	pObj->AddComponent(pCrosshair);

	pObj->Transform()->SetRelativePos(Vec3(0, 0, 0));
	pObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_UI, false);

	// CPausePanel
	pObj = new CGameObject;
	pObj->SetName(L"Pause Panel");

	pObj->AddComponent(new CTransform);

	auto pPausePanel = new CPausePanel;
	pObj->AddComponent(pPausePanel);

	pObj->Transform()->SetRelativePos(Vec3(0, 0, 0));
	pObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_UI, false);

	// Fire Btn
	pObj = new CGameObject;
	pObj->SetName(L"Fire Btn");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);

	auto SubHPbtnUI = new CBtnUIScript;
	pObj->AddComponent(SubHPbtnUI);
	SubHPbtnUI->AllowTexSet();
	SubHPbtnUI->SetNormalImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HP_Yellow.png"));
	SubHPbtnUI->SetDeletage((CEntity*)pPausePanel, (DelegateFunc)&CPausePanel::ActivePausePanel);

	pObj->Transform()->SetRelativePos(Vec3(890, 400, 100.f));
	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);
	pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0,
													CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HP_Yellow.png"));
	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_UI, false);
	
	// Fire Btn
	pObj = new CGameObject;
	pObj->SetName(L"Fire Btn2");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);

	auto SubHPbtnUI2 = new CBtnUIScript;
	pObj->AddComponent(SubHPbtnUI2);
	SubHPbtnUI2->AllowTexSet();
	SubHPbtnUI2->SetNormalImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HP_Yellow.png"));
	SubHPbtnUI2->SetDeletage((CEntity*)pPausePanel, (DelegateFunc)&CPausePanel::InactivePausePanel);

	pObj->Transform()->SetRelativePos(Vec3(-650, -400, 100.f));
	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);
	pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0,
													CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HP_Yellow.png"));
	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_UI, false);


	// Test Font
	pObj = new CGameObject;
	pObj->SetName(L"Test Font");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CFontRender);

	pObj->Transform()->SetRelativePos(Vec3(650, -400, 100.f));
	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	tFontInfo FontInfo{};
	FontInfo.Color = FONT_RGBA(255, 255, 255, 255);
	FontInfo.fFontSize = 50.f;
	FontInfo.FontType  = FONT_TYPE::ARIAL;
	FontInfo.WStr	   = L"ABCDEFGHIJKLMNOPQR";

	pObj->FontRender()->SetFontInfo(FontInfo);
	pObj->FontRender()->AllowConvertWorldPosToWindowPos(true);

	pTempLevel->AddObject(pObj, (UINT)LAYER::LAYER_UI, false);


	GamePlayStatic::ChangeLevel(pTempLevel, LEVEL_STATE::STOP);

	CLevelSaveLoad::SaveLevel(pTempLevel, L"level\\UITest.lv");
}