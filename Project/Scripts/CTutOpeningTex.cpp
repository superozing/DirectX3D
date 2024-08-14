#include "pch.h"
#include "CTutOpeningTex.h"

#include "CTutorialGameMode.h"

CTutOpeningTex::CTutOpeningTex()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::TUTOPENINGTEX)
	, m_LVState((int)TutorialState::END)
{
}

CTutOpeningTex::~CTutOpeningTex()
{
}

void CTutOpeningTex::begin()
{
	CAtlasImageUIScript::begin();

	m_UIImg			= CAssetMgr::GetInst()->Load<CTexture>(TEXModal_Bg);
	m_vBlendColor.w = 0.f;

	m_LVState = GetOwner()->GetScript<CTutorialGameMode>()->GetCurState();
}

void CTutOpeningTex::tick()
{
	CAtlasImageUIScript::tick();

	m_LVState = GetOwner()->GetScript<CTutorialGameMode>()->GetCurState();

	if ((int)TutorialState::OpeningIn == m_LVState)
	{
		float duration = GetOwner()->GetScript<CTutorialGameMode>()->GetOpeningInTime();
		float accTime  = GetOwner()->GetScript<CTutorialGameMode>()->GetAccTime();

		float fLerp = RoRMath::Lerp(0.f, 1.f, accTime / duration);

		m_vBlendColor.w = fLerp;
	}
	else if ((int)TutorialState::OpeningOut == m_LVState)
	{
		float duration = GetOwner()->GetScript<CTutorialGameMode>()->GetOpeningOutTime();
		float accTime  = GetOwner()->GetScript<CTutorialGameMode>()->GetAccTime();

		float fLerp = RoRMath::Lerp(0.f, 1.f, accTime / duration);

		m_vBlendColor.w = 1 - fLerp;
	}
}
