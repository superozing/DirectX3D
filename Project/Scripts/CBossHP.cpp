#include "pch.h"
#include "CBossHP.h"
#include <Engine/CLogMgr.h>
#include <Engine/CAssetMgr.h>

#include "CImageUIScript.h"
#include "CPanelUIScript.h"

namespace BOSSHP
{
#define SINGLE 0
#define ODD 1
#define EVEN 2

#define BAR_XSCALE 1125
#define BAR_YSCALE 44

#define BAR_XPOS 70
#define BAR_YPOS -10

#define SCALE_RATIO 0.7f

#define BASE CProgressBar
} // namespace BOSSHP

CBossHP::CBossHP()
	: CProgressBar((UINT)SCRIPT_TYPE::BOSSHP)
{
}

CBossHP::~CBossHP()
{
}

void CBossHP::begin()
{
	AppendScriptParam("Line HP", SCRIPT_PARAM::INT, &m_LineHP);

	CProgressBar::begin();
}

void CBossHP::tick()
{
	using namespace BOSSHP;

	int CurHP = GetCurHP();
	int MaxHP = GetMaxHP();

	m_CurLerpHP = RoRMath::Lerp(m_CurLerpHP, CurHP, DT * 0.1f);

	// 현재 남은 줄 개수와 렌더링 할 색상 구하기
	UINT  LineInfo		 = 0;
	float HPRatio		 = 0.f;
	float CurLerpHPRatio = 0.f;
	int	  LineCount		 = 0;

	if (MaxHP != 0)
	{
		// 현재 남은 HP가 라인 HP보다 작을 경우
		// 비율에 따라서 한 가지 색상의 라인만 출력해야 해요.
		if (m_LineHP > CurHP)
		{
			LineInfo = SINGLE;
		}
		else
		{
			// 현재 남은 줄 개수 계산 (만약 LineHP가 0이라면 LineHP를 1이라고 가정함)
			LineCount = (m_LineHP) ? CurHP / m_LineHP : CurHP;

			// 줄 개수의 홀짝 세팅
			LineInfo = LineCount % 2 == 1 ? EVEN : ODD;
		}

		// 체력 비율 계산
		HPRatio		   = float(CurHP - LineCount * m_LineHP) / m_LineHP;
		CurLerpHPRatio = float(m_CurLerpHP - LineCount * m_LineHP) / m_LineHP;
	}

	m_pHPLineUI->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, LineInfo);
	m_pHPLineUI->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, CurLerpHPRatio);
	m_pHPLineUI->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, HPRatio);
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
	Transform()->SetRelativeScale(Vec3(1388.f * SCALE_RATIO, 222.f * SCALE_RATIO, 1.f));

	// panel texture 설정
	GetPanelUI()->SetPanelTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Boss/Ingame_Raid_Boss_Gauge.png"));

	GetPanelUI()->SetUIType(UI_TYPE::BOSSHP);

	CGameObject* pObj = nullptr;

	// m_pPortrait
	pObj		= new CGameObject;
	m_pPortrait = new CImageUIScript;

	pObj->SetName(L"PortraitImg");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pPortrait);

	pObj->Transform()->SetRelativePos(Vec3(-553 * SCALE_RATIO, 10 * SCALE_RATIO, -90.f));
	pObj->Transform()->SetRelativeScale(Vec3(211.f * SCALE_RATIO, 215.f * SCALE_RATIO, 1.f));

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

	pObj->Transform()->SetRelativePos(Vec3(-553 * SCALE_RATIO, -80 * SCALE_RATIO, -80.f * SCALE_RATIO));
	pObj->Transform()->SetRelativeScale(Vec3(128.f * SCALE_RATIO, 32.f * SCALE_RATIO, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);

	m_pImgFont->SetUIImg(m_ImgFontTex);
	m_pImgFont->AllowBindTexPerFrame();

	GetOwner()->AddChild(pObj);

	// m_pHPLineUI
	pObj		= new CGameObject;
	m_pHPLineUI = new CImageUIScript;
	pObj->SetName(L"OddLineHP");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pHPLineUI);

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);
	pObj->MeshRender()->GetMaterial(0)->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BossHPShader"));
	pObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::BOOL_0, true);
	pObj->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, 10);
	pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0,
													CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HPLine_Div.png"));

	pObj->Transform()->SetRelativePos(Vec3(100 * SCALE_RATIO, -10 * SCALE_RATIO, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(1100.f * SCALE_RATIO, 44.f * SCALE_RATIO, 1.f));

	GetOwner()->AddChild(pObj);
}

#define TagLineHP "[Line HP]"
#define TagPortraitTex "[Portrait Tex]"
#define TagImgFontTex "[ImgFont Tex]"

void CBossHP::SaveToFile(FILE* _File)
{
}

void CBossHP::SaveToFile(ofstream& fout)
{
	fout << TagLineHP << endl;
	fout << m_LineHP << endl;

	fout << TagPortraitTex << endl;
	SaveAssetRef(m_PortraitTex, fout);

	fout << TagImgFontTex << endl;
	SaveAssetRef(m_ImgFontTex, fout);
}

void CBossHP::LoadFromFile(FILE* _File)
{
}

void CBossHP::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagLineHP);
	fin >> m_LineHP;

	Utils::GetLineUntilString(fin, TagPortraitTex);
	LoadAssetRef(m_PortraitTex, fin);

	Utils::GetLineUntilString(fin, TagImgFontTex);
	LoadAssetRef(m_ImgFontTex, fin);
}
