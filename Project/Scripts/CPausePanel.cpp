#include "pch.h"
#include "CPausePanel.h"

#include <Engine\CDevice.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CBtnUIScript.h"

CPausePanel::CPausePanel()
	: CImageUIScript((UINT)SCRIPT_TYPE::PAUSEPANEL)
	, m_vIncreaseSpeed(0.3f, 0.3f)
{
	AppendScriptParam("OpenSpeed", SCRIPT_PARAM::VEC2, &m_vIncreaseSpeed);
}

CPausePanel::~CPausePanel()
{
}

void CPausePanel::Continue()
{
	Draw(false);
	for (const auto& btn : m_vecBtns)
	{
		btn->Draw(false);
	}
}

void CPausePanel::Exit()
{
}

void CPausePanel::begin()
{
	Draw(false);

	// Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	//// 계속하기 버튼
	// auto pObj	   = new CGameObject;
	// m_pContinueBtn = new CBtnUIScript;

	// pObj->SetName("Continue Btn");

	// pObj->AddComponent(new CTransform);
	// pObj->AddComponent(new CMeshRender);
	// pObj->AddComponent(m_pContinueBtn);

	// pObj->Transform()->SetRelativePos(Vec3(300.f, 0.f, -10.f));
	// pObj->Transform()->SetRelativeScale(Vec3(250.f, 250.f, 1.f));

	// pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	// pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	// pObj->MeshRender()->GetDynamicMaterial(0);

	// m_pContinueBtn->SetDeletage(this, (DelegateFunc)&CPausePanel::InactivePausePanel);
	// m_pContinueBtn->SetNormalImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png"));
	// m_pContinueBtn->SetHoverImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png"));
	// m_pContinueBtn->SetPressedImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png"));
	// m_pContinueBtn->DisallowCallFunc();
	// m_pContinueBtn->DisallowTexSet();
	// m_pContinueBtn->SetUIType(UI_TYPE::PAUSEPANEL);

	// GetOwner()->AddChild(pObj);

	//// 나가기 버튼
	// pObj	   = new CGameObject;
	// m_pExitBtn = new CBtnUIScript;

	// pObj->SetName("Exit Btn");

	// pObj->AddComponent(new CTransform);
	// pObj->AddComponent(new CMeshRender);
	// pObj->AddComponent(m_pExitBtn);

	// pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	// pObj->Transform()->SetRelativeScale(Vec3(250.f, 250.f, 1.f));

	// pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	// pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	// pObj->MeshRender()->GetDynamicMaterial(0);

	// m_pExitBtn->SetDeletage(this, (DelegateFunc)&CPausePanel::ExitBtnDelegateFunc);
	// m_pExitBtn->SetNormalImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png"));
	// m_pExitBtn->SetHoverImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png"));
	// m_pExitBtn->SetPressedImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png"));
	// m_pExitBtn->DisallowCallFunc();
	// m_pExitBtn->DisallowTexSet();
	// m_pExitBtn->SetUIType(UI_TYPE::PAUSEPANEL);

	// GetOwner()->AddChild(pObj);

	//// m_PanelFontInfo
	// m_PanelFontInfo.Color	  = FONT_RGBA(120, 120, 180, 255);
	// m_PanelFontInfo.fFontSize = 50.f;

	// m_PanelFontInfo.FontType = FONT_TYPE::MAPLE;

	// m_PanelFontInfo.vPos = Vec2(vResol.x / 2, -250.f + (vResol.y / 2));

	// m_PanelFontInfo.TextFlag = FW1_CENTER;
	// m_PanelFontInfo.WStr	 = L"일시 정지";
}

void CPausePanel::tick()
{
	CImageUIScript::tick();
	static float xTimer = 0.f;
	static float yTimer = 0.f;

	Vec2 vRes = CDevice::GetInst()->GetRenderResolution();

	// 열리기 시작
	if (KEY_TAP(ESC) && !IsDraw())
	{
		Draw(true);
		Transform()->SetRelativeScale(Vec3(10.f, 10.f, 1.f));

		CTimeMgr::GetInst()->SetDTScale(0.f);

		m_bIncreaseX = true;
		xTimer		 = 0.f;
		GetOwner()->Transform()->Lerp(Vec3(), false, Vec3(), true, Vec3(vRes.x - 350.f, 10.f, 1.f), m_vIncreaseSpeed.x);
	}

	// x축 크기 증가시작
	if (m_bIncreaseX && !m_bIncreaseY)
	{
		xTimer += DT_ENGINE;
		// x축 크기 증가완료 y축 크기 증가 시작
		if (xTimer >= m_vIncreaseSpeed.x)
		{
			yTimer		 = 0.f;
			m_bIncreaseY = true;
			Transform()->Lerp(Vec3(), false, Vec3(), true, Vec3(Transform()->GetRelativeScale().x, vRes.y - 250.f, 1.f),
							  m_vIncreaseSpeed.y);
		}
	}

	// y축 크기 증가 시작
	if (m_bIncreaseX && m_bIncreaseY)
	{
		yTimer += DT_ENGINE;
		// x축 크기 증가 완료, y축 크기 증가 완료
		if (yTimer >= m_vIncreaseSpeed.y)
		{
			for (const auto& btn : m_vecBtns)
			{
				btn->Draw(true);
			}
		}
	}
}

void CPausePanel::SaveToFile(ofstream& fout)
{
	CImageUIScript::SaveToFile(fout);
}

void CPausePanel::LoadFromFile(ifstream& fin)
{
	CImageUIScript::LoadFromFile(fin);
}
//
// void CPausePanel::ActivePausePanel()
//{
//	m_pPanelUI->AllowTexSet();
//	m_pPanelUI->EnableMouseInput();
//
//	m_pContinueBtn->AllowCallFunc();
//	m_pContinueBtn->AllowTexSet();
//
//	m_pExitBtn->AllowCallFunc();
//	m_pExitBtn->AllowTexSet();
//
//	m_bActivate = true;
//
//	CLevelMgr::GetInst()->GetCurrentLevel()->ChangeModalState(true);
//
//	for (int i = 0; i < (UINT)UI_TYPE::END; ++i)
//	{
//		m_tempActiveUIType[i] = CUIMgr::GetInst()->IsActiveUIType((UI_TYPE)i);
//		CUIMgr::GetInst()->InactiveUIType((UI_TYPE)i);
//	}
//
//	CUIMgr::GetInst()->ActiveUIType(UI_TYPE::PAUSEPANEL);
//}
//
// void CPausePanel::InactivePausePanel()
//{
//	// m_pModalBg->SetUIImg(nullptr);
//	// m_pModalBg->BindUIImgOnTexParam();
//	// m_pModalBg->DisableMouseInput();
//
//	m_pPanelUI->DisallowTexSet();
//	m_pPanelUI->DisableMouseInput();
//
//	m_pContinueBtn->DisallowCallFunc();
//	m_pContinueBtn->DisallowTexSet();
//
//	m_pExitBtn->DisallowCallFunc();
//	m_pExitBtn->DisallowTexSet();
//
//	m_bActivate = false;
//
//	CLevelMgr::GetInst()->GetCurrentLevel()->ChangeModalState(false);
//
//	CUIMgr::GetInst()->InactiveUIType(UI_TYPE::PAUSEPANEL);
//
//	for (int i = 0; i < (UINT)UI_TYPE::END; ++i)
//	{
//		if (m_tempActiveUIType[i])
//			CUIMgr::GetInst()->ActiveUIType((UI_TYPE)i);
//	}
//}
//
// void CPausePanel::SetParentPanelUI()
//{
//	auto pOwn = GetOwner();
//
//	pOwn->Transform()->SetRelativePos(Vec3(0.f, 0.f, -500.f));
//	pOwn->Transform()->SetRelativeScale(Vec3(1024.f, 512.f, 1.f));
//
//	m_pPanelUI = pOwn->GetScript<CPanelUIScript>();
//
//	if (!m_pPanelUI)
//	{
//		m_pPanelUI = new CPanelUIScript;
//		pOwn->AddComponent(m_pPanelUI);
//	}
//
//	m_pPanelUI->DisableMouseInput();
//	m_pPanelUI->DisallowDragAndDrop();
//	m_pPanelUI->DisallowTexSet();
//	m_pPanelUI->SetUIType(UI_TYPE::PAUSEPANEL);
//
//	auto meshrender = pOwn->MeshRender();
//
//	if (!meshrender)
//	{
//		meshrender = new CMeshRender;
//		pOwn->AddComponent(meshrender);
//	}
//
//	meshrender->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
//	meshrender->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
//	meshrender->GetDynamicMaterial(0);
//
//	m_pPanelUI->SetPanelTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Pause_Bg.png"));
//}
