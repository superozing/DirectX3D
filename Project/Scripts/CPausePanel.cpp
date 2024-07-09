#include "pch.h"
#include "CPausePanel.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CFontMgr.h>
#include <Engine/CDevice.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CImageUIScript.h"
#include "CBtnUIScript.h"
#include "CPanelUIScript.h"

CPausePanel::CPausePanel()
	: CScript((UINT)SCRIPT_TYPE::PAUSEPANEL)
{
}

CPausePanel::~CPausePanel()
{
}

void CPausePanel::begin()
{
	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	// m_pModalBg
	auto pObj  = new CGameObject;
	m_pModalBg = new CImageUIScript;

	pObj->SetName("Modal Background");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pModalBg);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 10.f));
	pObj->Transform()->SetRelativeScale(Vec3(vResol.x, vResol.y, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	m_pModalBg->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Modal_Bg.png"));
	m_pModalBg->DisallowBindTexPerFrame();

	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj, (UINT)LAYER::LAYER_UI);

	// m_pPausePanel
	m_pPausePanelObject = new CGameObject;
	m_pPausePanel		= new CPanelUIScript;

	m_pPausePanelObject->SetName("Pause Panel");

	m_pPausePanelObject->AddComponent(new CTransform);
	m_pPausePanelObject->AddComponent(new CMeshRender);
	m_pPausePanelObject->AddComponent(m_pPausePanel);

	m_pPausePanelObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 20.f));
	m_pPausePanelObject->Transform()->SetRelativeScale(Vec3(1024.f, 512.f, 1.f));

	m_pPausePanelObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	m_pPausePanelObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	m_pPausePanelObject->MeshRender()->GetDynamicMaterial(0);

	m_pPausePanel->SetPanelTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Pause_Bg.png"));
	m_pPausePanel->DisallowTexSet();
	m_pPausePanel->DisallowDragAndDrop();

	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(m_pPausePanelObject, (UINT)LAYER::LAYER_UI);

	// m_PanelFontInfo
	m_PanelFontInfo.Color	  = FONT_RGBA(20, 20, 180, 255);
	m_PanelFontInfo.fFontSize = 50.f;

	m_PanelFontInfo.FontType = FONT_TYPE::MAPLE;

	m_PanelFontInfo.vPos  = Vec2(vResol.x / 2, -250.f + (vResol.y / 2));

	m_PanelFontInfo.TextFlag = FW1_CENTER;
	m_PanelFontInfo.WStr	 = L"일시 정지";

	// m_pExitBtn
	pObj	   = new CGameObject;
	m_pExitBtn = new CBtnUIScript;

	pObj->SetName("Modal Background");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pExitBtn);

	pObj->Transform()->SetRelativePos(Vec3(450.f, 200.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	m_pExitBtn->SetDeletage(this, (DelegateFunc)&CPausePanel::InactivePausePanel);
	m_pExitBtn->SetNormalImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png"));
	m_pExitBtn->SetHoverImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png"));
	m_pExitBtn->SetPressedImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png"));
	m_pExitBtn->DisallowCallFunc();
	m_pExitBtn->DisallowTexSet();

	m_pPausePanelObject->AddChild(pObj);

	// m_pFuncBtn

	// for (int i = 0; i < 2; ++i)
	//{
	//	auto pObj = new CGameObject;
	//	m_pFuncBtn[i] = new CBtnUIScript;
	// }

	// 1. 콜백 버튼을 추가할 함수 필요
	// 2.
}

void CPausePanel::tick()
{
	if (m_bActivate)
	{
		CFontMgr::GetInst()->RegisterFont(m_PanelFontInfo);
	}
}

void CPausePanel::ActivePausePanel()
{
	m_pModalBg->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Modal_Bg.png"));
	m_pModalBg->BindUIImgOnTexParam();
	m_pModalBg->EnableMouseInput();

	m_pPausePanel->AllowTexSet();
	m_pPausePanel->EnableMouseInput();

	m_pExitBtn->AllowCallFunc();
	m_pExitBtn->AllowTexSet();

	m_bActivate = true;

	CLevelMgr::GetInst()->GetCurrentLevel()->ChangeModalState(true);
}

void CPausePanel::InactivePausePanel()
{
	m_pModalBg->SetUIImg(nullptr);
	m_pModalBg->BindUIImgOnTexParam();
	m_pModalBg->DisableMouseInput();

	m_pPausePanel->DisallowTexSet();
	m_pPausePanel->DisableMouseInput();

	m_pExitBtn->DisallowCallFunc();
	m_pExitBtn->DisallowTexSet();

	m_bActivate = false;

	CLevelMgr::GetInst()->GetCurrentLevel()->ChangeModalState(false);
}
