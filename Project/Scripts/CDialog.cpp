#include "pch.h"
#include "CDialog.h"

#include "CPanelUIScript.h"
#include "CImageUIScript.h"
#include <Engine/CDevice.h>
#include <Engine/CUIMgr.h>
#include <Engine/CFontMgr.h>

CDialog::CDialog()
	: CScript((UINT)SCRIPT_TYPE::DIALOG)
{
}

CDialog::~CDialog()
{
}

void CDialog::begin()
{
	// AppendScriptParam("Dialog String", SCRIPT_PARAM::FLOAT, &m_CrosshairColor.x);

	// CDialog를 소유한 오브젝트에게 패널 UI 부착
	SetParentPanelUI();

	// CharatorUI
	CGameObject* pObj = new CGameObject;
	m_pCharatorUI	  = new CImageUIScript;

	pObj->SetName(L"Charator Image UI");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pCharatorUI);

	m_pCharatorUI->SetUIType(UI_TYPE::DIALOG);

	// CharatorTex가 Set 되어있을 경우.
	if (m_pCharatorTex.Get())
		m_pCharatorUI->SetUIImg(m_pCharatorTex);
	else // 만약 세팅되어있지 않을 경우에는 기본 이미지를 넣어주어요.
		m_pCharatorUI->SetUIImg(
			CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Arona/Operator_Portrait_Arona_00.png"));

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	GetOwner()->AddChild(pObj);

	// m_pBgUI
	pObj	= new CGameObject;
	m_pBgUI = new CImageUIScript;

	pObj->SetName(L"Bg");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pBgUI);

	m_pBgUI->SetUIType(UI_TYPE::DIALOG);

	// 배경 이미지가 Set 되어있을 경우.
	if (m_pBgTex.Get())
		m_pBgUI->SetUIImg(m_pBgTex);
	else // 만약 세팅되어있지 않을 경우에는 기본 이미지를 넣어주어요.
		m_pBgUI->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Bg/Event_Main_Shop_Bg_817.png"));

	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -200.f));
	pObj->Transform()->SetRelativeScale(Vec3(vResol, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	GetOwner()->AddChild(pObj);

	// m_pFontBgUI
	pObj		= new CGameObject;
	m_pFontBgUI = new CImageUIScript;

	pObj->SetName(L"Font Bg");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pFontBgUI);

	m_pFontBgUI->SetUIType(UI_TYPE::DIALOG);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -200.f));
	pObj->Transform()->SetRelativeScale(Vec3(vResol.x, 200.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);
	pObj->MeshRender()->GetMaterial(0)->SetShader(
		CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"DialogFontBgShader"));

	GetOwner()->AddChild(pObj);

	//// m_pDividingLineUI
	// CGameObject* pObj = new CGameObject;
	// m_pDividingLineUI = new CImageUIScript;

	// pObj->SetName(L"Dividing Line");
	// pObj->AddComponent(new CTransform);
	// pObj->AddComponent(new CMeshRender);
	// pObj->AddComponent(m_pDividingLineUI);

	// m_pDividingLineUI->SetUIType(UI_TYPE::DIALOG);

	// pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -200.f));
	// pObj->Transform()->SetRelativeScale(Vec3(vResol.x, 200.f, 1.f));

	// pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	// pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	// pObj->MeshRender()->GetDynamicMaterial(0);

	// GetOwner()->AddChild(pObj);

	// FontInfo
	m_CharactorNameFontInfo.Color	  = FONT_RGBA(123, 210, 253, 255);
	m_CharactorNameFontInfo.fFontSize = 60.f;
	m_CharactorNameFontInfo.FontType  = FONT_TYPE::MAIN_BOLD;
	m_CharactorNameFontInfo.WStr	  = L"아로나";

	// FontInfo
	m_DialogFontInfo.Color	   = FONT_RGBA(255, 255, 255, 255);
	m_DialogFontInfo.fFontSize = 50.f;
	m_DialogFontInfo.FontType  = FONT_TYPE::MAIN_BOLD;
	m_DialogFontInfo.WStr	   = L"안녕하세요";
}

void CDialog::tick()
{
	// 현재 UI가 활성화 상태일 경우에만 폰트 set 해주기.
	if (CUIMgr::GetInst()->IsActiveUIType(UI_TYPE::DIALOG))
	{
		CFontMgr::GetInst()->RegisterFont(m_CharactorNameFontInfo);
		CFontMgr::GetInst()->RegisterFont(m_DialogFontInfo);
	}
}

void CDialog::SetParentPanelUI()
{
	m_pPanelUI = GetOwner()->GetScript<CPanelUIScript>();

	if (!m_pPanelUI)
	{
		m_pPanelUI = new CPanelUIScript;
		GetOwner()->AddComponent(m_pPanelUI);
	}

	m_pPanelUI->DisableMouseInput();
	m_pPanelUI->DisallowDragAndDrop();
	m_pPanelUI->DisallowTexSet();
	m_pPanelUI->SetUIType(UI_TYPE::DIALOG);

	if (!GetOwner()->MeshRender())
	{
		GetOwner()->AddComponent(new CMeshRender);
	}

	GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	GetOwner()->MeshRender()->GetDynamicMaterial(0);
}

void CDialog::SaveToFile(FILE* _File)
{
}

void CDialog::SaveToFile(ofstream& fout)
{
}

void CDialog::LoadFromFile(FILE* _File)
{
}

void CDialog::LoadFromFile(ifstream& fin)
{
}
