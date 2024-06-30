#include "pch.h"
#include "CBossHP.h"
#include <Engine/CLogMgr.h>
#include <Engine/CAssetMgr.h>

#include "CImageUIScript.h"
#include "CPanelUIScript.h"

namespace BOSSHP
{

#define BAR_XSCALE 1100
#define BAR_YSCALE 44

#define BAR_XPOS 100
#define BAR_YPOS -10

#define BASE CProgressBar
}

CBossHP::CBossHP()
	: CProgressBar((UINT)SCRIPT_TYPE::BOSSHP)
{
}

CBossHP::~CBossHP()
{
}

void CBossHP::begin()
{
	CProgressBar::begin();

	MakeChildObjects();
}

void CBossHP::tick()
{
	using namespace BOSSHP;

	int CurHP = GetCurHP();
	int MaxHP = GetMaxHP();

	//SetCurHP(RoRMath::Lerp(CurHP, 100, DT * 5));

	if (MaxHP != 0)
	{
		// 1. 현재 체력 비율 계산하기
		float fHPRatio = (float)CurHP / MaxHP;

		// 2. 체력 비율에 따른 HP UI transform 조절
		m_pHPLineUI->Transform()->SetRelativeScale(Vec3(fHPRatio * BAR_XSCALE, BAR_YSCALE, 1));
		m_pHPLineUI->Transform()->SetRelativePos(Vec3(BAR_XPOS - ((1 - fHPRatio) * BAR_XSCALE) / 2, BAR_YPOS, 0));
	}
}

void CBossHP::SetLineHP(int _LineHP)
{
	using namespace BOSSHP;

	int CurHP = GetCurHP();
	int MaxHP = GetMaxHP();

	if (MaxHP < _LineHP)
	{
		CLogMgr::GetInst()->AddLog(Log_Level::WARN, L"Line HP가 Max HP보다 큽니다.");
		_LineHP = MaxHP;
	}

	m_LineHP = _LineHP;
}

void CBossHP::SetPortraitTex(Ptr<CTexture> _PortraitTex)
{
	m_PortraitTex = _PortraitTex;

	if (m_pPortrait)
	{
		m_pPortrait->SetUIImg(m_PortraitTex);
	}
}

void CBossHP::SetImgFontTex(Ptr<CTexture> _ImgFontTex)
{
	m_ImgFontTex = _ImgFontTex;

	if (m_pImgFont)
	{
		m_pImgFont->SetUIImg(m_ImgFontTex);
	}
}

void CBossHP::Add100()
{
	SetCurHP(GetCurHP() + 100);
}

void CBossHP::Sub100()
{
	SetCurHP(GetCurHP() - 100);
}

void CBossHP::MakeChildObjects()
{
	using namespace BOSSHP;

	// transform 조정
	Transform()->SetRelativePos(Vec3(0, 0, -100.f));
	Transform()->SetRelativeScale(Vec3(1388.f, 222.f, 1.f));

	// panel texture 설정
	GetPanelUI()->SetPanelTex(
		CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Ingame_Raid_Boss_Gauge.png"));


	CGameObject* pObj = nullptr;

	// m_pPortrait
	pObj		= new CGameObject;
	m_pPortrait = new CImageUIScript;

	pObj->SetName(L"PortraitImg");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pPortrait);

	pObj->Transform()->SetRelativePos(Vec3(-553, 10, -90.f));
	pObj->Transform()->SetRelativeScale(Vec3(211.f, 215.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	m_pPortrait->SetUIImg(m_PortraitTex);
	m_pPortrait->AllowBindTexPerFrame();

	GetOwner()->AddChild(pObj);

	// m_pImgFont
	pObj	   = new CGameObject;
	m_pImgFont = new CImageUIScript;

	pObj->SetName(L"FontImg");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pImgFont);

	pObj->Transform()->SetRelativePos(Vec3(-553, -80, -80.f));
	pObj->Transform()->SetRelativeScale(Vec3(128.f, 32.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	m_pImgFont->SetUIImg(m_ImgFontTex);
	m_pImgFont->AllowBindTexPerFrame();

	GetOwner()->AddChild(pObj);

	// m_pHPLineUI
	pObj		   = new CGameObject;
	m_pHPLineUI = new CImageUIScript;
	pObj->SetName(L"OddLineHP");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pHPLineUI);

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	m_pHPLineUI->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HP_Red.png"));
	m_pHPLineUI->AllowBindTexPerFrame();

	pObj->Transform()->SetRelativePos(Vec3(100, -10, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(1100.f, 44.f, 1.f));

	GetOwner()->AddChild(pObj);
}
