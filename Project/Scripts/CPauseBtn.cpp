#include "pch.h"
#include "CPauseBtn.h"
#include "CBtnUIScript.h"
#include "CPausePanel.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CDevice.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CPauseBtn::CPauseBtn()
	: CScript((UINT)SCRIPT_TYPE::PAUSEBTN)
{
}

CPauseBtn::~CPauseBtn()
{
}

void CPauseBtn::begin()
{
	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	// m_pPauseBtnUI
	auto pObj	  = new CGameObject;
	m_pPauseBtnUI = new CBtnUIScript;

	pObj->SetName("Pause Btn");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pPauseBtnUI);

	pObj->Transform()->SetRelativePos(Vec3(vResol.x / 2 - 60.f, vResol.y / 2 - 100.f, 10.f));
	pObj->Transform()->SetRelativeScale(Vec3(100, 100, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	auto BtnTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png");
	m_pPauseBtnUI->SetNormalImg(BtnTex);
	m_pPauseBtnUI->SetHoverImg(BtnTex);
	m_pPauseBtnUI->SetPressedImg(BtnTex);
	m_pPauseBtnUI->AllowCallFunc();
	m_pPauseBtnUI->AllowTexSet();
	m_pPauseBtnUI->EnableMouseInput();
	m_pPauseBtnUI->SetDeletage(m_pPausePanel, (DelegateFunc)&CPausePanel::ActivePausePanel);

	m_pPauseBtnUI->SetUIType(UI_TYPE::PAUSEBTN);

	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj, (UINT)LAYER::LAYER_UI);
}

void CPauseBtn::tick()
{
}
