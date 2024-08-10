#include "pch.h"
#include "CWarningBGTex.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include "CBossLV.h"

CWarningBGTex::CWarningBGTex()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::WARNINGBGTEX)
	, m_BossLV(nullptr)
	, m_LVState((int)BossLV_STATE::END)
	, m_Pos{}
	, m_Scale{}
	, m_Rot{}
{
}

CWarningBGTex::~CWarningBGTex()
{
}

void CWarningBGTex::begin()
{
	CAtlasImageUIScript::begin();

	m_UIImg = CAssetMgr::GetInst()->Load<CTexture>(TEXIngame_Warning_Text_1);
	SetAtlasSize(Vec2(0.f, 0.f), Vec2(m_UIImg->GetWidth(), m_UIImg->GetHeight()));
	m_vBlendColor = Vec4(1.f, 1.f, 0.f, 0.f);

	m_BossLV = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"GameMode");
}

void CWarningBGTex::tick()
{
	CAtlasImageUIScript::tick();

	m_LVState = GetOwner()->GetScript<CBossLV>()->GetCurLVState();

	if ((int)BossLV_STATE::OpeningDelay == m_LVState)
	{
		float duration = m_BossLV->GetScript<CBossLV>()->GetOpeningInTime();
		float accTime  = m_BossLV->GetScript<CBossLV>()->GetAccTime();

		float fLerp = RoRMath::Lerp(0.f, 1.f, accTime / duration);

		m_vBlendColor.w = fLerp;
	}
}
