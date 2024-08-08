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

#define BOSS_NAME L"KAITEN FX Mk.0"

#define BASE CProgressBar
} // namespace BOSSHP

CBossHP::CBossHP()
	: CProgressBar((UINT)SCRIPT_TYPE::BOSSHP)
{
}

CBossHP::~CBossHP()
{
}

static int tempDamageBuffer = 0;

void CBossHP::begin()
{
	CProgressBar::begin();

	// 디버그 용 멤버 함수 등록
	AppendMemberFunction("Add 100 HP", SCRIPT_PARAM::FUNC_MEMBER, "Add 100 HP", std::bind(&CBossHP::Add100, this));
	AppendMemberFunction("Sub 100 HP", SCRIPT_PARAM::FUNC_MEMBER, "Sub 100 HP", std::bind(&CBossHP::Sub100, this));
	AppendScriptParam("Damage Value", SCRIPT_PARAM::INT, &tempDamageBuffer);
	AppendMemberFunction("Append Damage", SCRIPT_PARAM::FUNC_MEMBER, "+ -> Damage, - -> Heal", std::bind(&CBossHP::DbgAppendDamageWrap, this));

	// 임시로 HP 값 세팅
	SetMaxHP(20000);
	SetLineHP(1000);
	SetCurHP(20000);
}

void CBossHP::tick()
{
	using namespace BOSSHP;

	int CurHP = GetCurHP();
	int MaxHP = GetMaxHP();

	m_CurLerpHP = RoRMath::Lerp(m_CurLerpHP, CurHP, DT * 6.f);

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

void CBossHP::Add100()
{
	SetCurHP(GetCurHP() + 100);
}

void CBossHP::Sub100()
{
	SetCurHP(GetCurHP() - 100);
}

void CBossHP::AppendDamage(int _Damage)
{
	SetCurHP(GetCurHP() - _Damage);
}

void CBossHP::DbgAppendDamageWrap()
{
	AppendDamage(tempDamageBuffer);
}

void CBossHP::MakeChildObjects()
{
	// Child Object Vector를 가져와 이름에 따라서 멤버로 세팅하기
	auto& vecChild = GetOwner()->GetChild();
	for (size_t i = 0; i < vecChild.size(); i++)
	{
		auto& name = vecChild[i]->GetName();
		if (name == L"Portrait")
		{
			m_pPortrait = vecChild[i]->GetScript<CImageUIScript>();
			continue;
		}
		else if (name == L"FontImg")
		{
			m_pImgFont = vecChild[i]->GetScript<CImageUIScript>();
			continue;
		}		
		else if (name == L"HPLine")
		{
			m_pHPLineUI = vecChild[i]->GetScript<CImageUIScript>();
			continue;
		}
	}

	// panel texture 설정
	GetPanelUI()->SetPanelTex(
		CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Boss/Ingame_Raid_Boss_Gauge.png"));
	GetPanelUI()->SetUIType(UI_TYPE::BOSSHP);

	CGameObject* pObj = nullptr;

	// m_pPortrait 이미지 세팅
	m_pPortrait->MeshRender()->GetDynamicMaterial(0);
	m_pPortrait->SetUIImg(
		CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/Portrait_Raidboss_KaitenRanger_Insane.png"));
	m_pPortrait->AllowBindTexPerFrame();

	// m_pImgFont 이미지 세팅
	m_pImgFont->MeshRender()->GetDynamicMaterial(0);
	m_pImgFont->SetUIImg(
		CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/ImgFont/ImageFont_Raidboss.png"));
	m_pImgFont->AllowBindTexPerFrame();

	// m_pHPLineUI 이미지 세팅
	m_pHPLineUI->SetUIImg(CAssetMgr::GetInst()->Load<CTexture>(L"texture/ui/HPLine_Div.png"));
	m_pHPLineUI->AllowBindTexPerFrame();

	m_pHPLineUI->MeshRender()->GetDynamicMaterial(0);
	m_pHPLineUI->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::BOOL_0, true);
	m_pHPLineUI->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, 10);
}

#define TagLineHP "[Line HP]"

void CBossHP::SaveToFile(FILE* _File)
{
}

void CBossHP::SaveToFile(ofstream& fout)
{
	fout << TagLineHP << endl;
	fout << m_LineHP << endl;
}

void CBossHP::LoadFromFile(FILE* _File)
{
}

void CBossHP::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagLineHP);
	fin >> m_LineHP;
}
