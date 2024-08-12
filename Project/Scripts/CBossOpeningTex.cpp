#include "pch.h"
#include "CBossOpeningTex.h"

#include "CBossLV.h"

CBossOpeningTex::CBossOpeningTex()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::BOSSOPENINGTEX)
	, m_LVState((int)BossLV_STATE::END)
{
}

CBossOpeningTex::~CBossOpeningTex()
{
}

void CBossOpeningTex::begin()
{
	CAtlasImageUIScript::begin();

	m_UIImg			= CAssetMgr::GetInst()->Load<CTexture>(TEXModal_Bg);
	m_vBlendColor.w = 0.f;

	m_LVState = GetOwner()->GetScript<CBossLV>()->GetCurLVState();
}

void CBossOpeningTex::tick()
{
	CAtlasImageUIScript::tick();

	m_LVState = GetOwner()->GetScript<CBossLV>()->GetCurLVState();

	if ((int)BossLV_STATE::OpeningIn == m_LVState)
	{
		float duration = GetOwner()->GetScript<CBossLV>()->GetOpeningInTime();
		float accTime  = GetOwner()->GetScript<CBossLV>()->GetAccTime();

		float fLerp = RoRMath::Lerp(0.f, 1.f, accTime / duration);

		m_vBlendColor.w = fLerp;
	}
	else if ((int)BossLV_STATE::OpeningOut == m_LVState)
	{
		float duration = GetOwner()->GetScript<CBossLV>()->GetOpeningOutTime();
		float accTime  = GetOwner()->GetScript<CBossLV>()->GetAccTime();

		float fLerp = RoRMath::Lerp(0.f, 1.f, accTime / duration);

		m_vBlendColor.w = 1 - fLerp;
	}
}
