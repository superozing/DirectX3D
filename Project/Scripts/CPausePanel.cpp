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
	SetParentPanelUI();

	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	//// m_pModalBg
	// auto pObj  = new CGameObject;
	// m_pModalBg = new CImageUIScript;

	// pObj->SetName("Modal Background");

	// pObj->AddComponent(new CTransform);
	// pObj->AddComponent(new CMeshRender);
	// pObj->AddComponent(m_pModalBg);

	// pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	// pObj->Transform()->SetRelativeScale(Vec3(vResol.x, vResol.y, 1.f));

	// pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	// pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	// pObj->MeshRender()->GetDynamicMaterial(0);

	// m_pModalBg->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Modal_Bg.png"));
	// m_pModalBg->DisallowBindTexPerFrame();

	// GetOwner()->AddChild(pObj);

	// 계속하기 버튼
	auto pObj	   = new CGameObject;
	m_pContinueBtn = new CBtnUIScript;

	pObj->SetName("Continue Btn");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pContinueBtn);

	pObj->Transform()->SetRelativePos(Vec3(300.f, 0.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(250.f, 250.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	m_pContinueBtn->SetDeletage(this, (DelegateFunc)&CPausePanel::InactivePausePanel);
	m_pContinueBtn->SetNormalImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png"));
	m_pContinueBtn->SetHoverImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png"));
	m_pContinueBtn->SetPressedImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png"));
	m_pContinueBtn->DisallowCallFunc();
	m_pContinueBtn->DisallowTexSet();

	GetOwner()->AddChild(pObj);

	// 나가기 버튼
	pObj	   = new CGameObject;
	m_pExitBtn = new CBtnUIScript;

	pObj->SetName("Exit Btn");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pExitBtn);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(250.f, 250.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	m_pExitBtn->SetDeletage(this, (DelegateFunc)&CPausePanel::ExitBtnDelegateFunc);
	m_pExitBtn->SetNormalImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png"));
	m_pExitBtn->SetHoverImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png"));
	m_pExitBtn->SetPressedImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png"));
	m_pExitBtn->DisallowCallFunc();
	m_pExitBtn->DisallowTexSet();

	GetOwner()->AddChild(pObj);

	// m_PanelFontInfo
	m_PanelFontInfo.Color	  = FONT_RGBA(120, 120, 180, 255);
	m_PanelFontInfo.fFontSize = 50.f;

	m_PanelFontInfo.FontType = FONT_TYPE::MAPLE;

	m_PanelFontInfo.vPos = Vec2(vResol.x / 2, -250.f + (vResol.y / 2));

	m_PanelFontInfo.TextFlag = FW1_CENTER;
	m_PanelFontInfo.WStr	 = L"일시 정지";
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
	// m_pModalBg->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Modal_Bg.png"));
	// m_pModalBg->BindUIImgOnTexParam();
	// m_pModalBg->EnableMouseInput();

	m_pPanelUI->AllowTexSet();
	m_pPanelUI->EnableMouseInput();

	m_pContinueBtn->AllowCallFunc();
	m_pContinueBtn->AllowTexSet();

	m_pExitBtn->AllowCallFunc();
	m_pExitBtn->AllowTexSet();

	m_bActivate = true;

	CLevelMgr::GetInst()->GetCurrentLevel()->ChangeModalState(true);
}

void CPausePanel::InactivePausePanel()
{
	// m_pModalBg->SetUIImg(nullptr);
	// m_pModalBg->BindUIImgOnTexParam();
	// m_pModalBg->DisableMouseInput();

	m_pPanelUI->DisallowTexSet();
	m_pPanelUI->DisableMouseInput();

	m_pContinueBtn->DisallowCallFunc();
	m_pContinueBtn->DisallowTexSet();

	m_pExitBtn->DisallowCallFunc();
	m_pExitBtn->DisallowTexSet();

	m_bActivate = false;

	CLevelMgr::GetInst()->GetCurrentLevel()->ChangeModalState(false);
}

void CPausePanel::SetParentPanelUI()
{
	auto pOwn = GetOwner();

	pOwn->Transform()->SetRelativePos(Vec3(0.f, 0.f, -500.f));
	pOwn->Transform()->SetRelativeScale(Vec3(1024.f, 512.f, 1.f));

	m_pPanelUI = pOwn->GetScript<CPanelUIScript>();

	if (!m_pPanelUI)
	{
		m_pPanelUI = new CPanelUIScript;
		pOwn->AddComponent(m_pPanelUI);
	}

	m_pPanelUI->DisableMouseInput();
	m_pPanelUI->DisallowDragAndDrop();
	m_pPanelUI->DisallowTexSet();

	auto meshrender = pOwn->MeshRender();

	if (!meshrender)
	{
		meshrender = new CMeshRender;
		pOwn->AddComponent(meshrender);
	}

	meshrender->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	meshrender->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	meshrender->GetDynamicMaterial(0);

	m_pPanelUI->SetPanelTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Pause_Bg.png"));
}
