#include "pch.h"
#include "CPauseBtn.h"
#include "CBtnUIScript.h"
#include "CPausePanel.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CDevice.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CEngine.h>

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

	Ptr<CTexture> BtnTex = m_PauseBtnTex;

	// 버튼 텍스쳐가 없을 경우 기본 이미지 세팅하기
	if (!BtnTex.Get())
		BtnTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Field_Common_Bg_03.png");

	m_pPauseBtnUI->SetNormalImg(BtnTex);
	m_pPauseBtnUI->SetHoverImg(BtnTex);
	m_pPauseBtnUI->SetPressedImg(BtnTex);
	m_pPauseBtnUI->AllowCallFunc();
	m_pPauseBtnUI->AllowTexSet();
	m_pPauseBtnUI->EnableMouseInput();

	// 만약 PausePanel이 세팅되지 않았다면 assert
	if (!m_pPausePanel)
	{
		MessageBox(CEngine::GetInst()->GetMainWind(),
				   L"PauseBtn에 PausePanel이 설정되지 않았습니다. \n PausePanel을 SetPausePanel()을 통해 Set 해주세요.",
				   L"Delegate 설정 실패", NULL);
		assert(nullptr);
	}

	m_pPauseBtnUI->SetDeletage(m_pPausePanel, (DelegateFunc)&CPausePanel::ActivePausePanel);

	m_pPauseBtnUI->SetUIType(UI_TYPE::PAUSEBTN);

	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj, (UINT)LAYER::LAYER_UI);
}

void CPauseBtn::tick()
{
}

#define TagFontImg "[Font Img]"

void CPauseBtn::SaveToFile(FILE* _File)
{
}

void CPauseBtn::SaveToFile(ofstream& fout)
{
	fout << TagFontImg << endl;
	if (m_PauseBtnTex.Get())
		SaveAssetRef(m_PauseBtnTex, fout);
	else
		fout << 0 << endl;
}

void CPauseBtn::LoadFromFile(FILE* _File)
{
}

void CPauseBtn::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagFontImg);
	LoadAssetRef(m_PauseBtnTex, fin);
}
