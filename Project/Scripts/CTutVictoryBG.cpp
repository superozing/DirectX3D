#include "pch.h"
#include "CTutVictoryBG.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include "CTutorialGameMode.h"

CTutVictoryBG::CTutVictoryBG()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::TUTVICTORYBG)
	, m_pGM(nullptr)
	, m_LVState((int)TutorialState::END)
{
}

CTutVictoryBG::~CTutVictoryBG()
{
}

void CTutVictoryBG::begin()
{
	CAtlasImageUIScript::begin();

	m_UIImg = CAssetMgr::GetInst()->Load<CTexture>(TEXModal_Bg);
	SetAtlasSize(Vec2(0.f, 0.f), Vec2(m_UIImg->GetWidth(), m_UIImg->GetHeight()));
	m_vBlendColor.w = 0.f;

	m_pGM = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"GameMode")->GetScript<CTutorialGameMode>();
}

void CTutVictoryBG::tick()
{
	CAtlasImageUIScript::tick();

	m_LVState = m_pGM->GetCurState();

	if ((int)TutorialState::EndingIn == m_LVState)
	{
		float duration = m_pGM->GetEndingInTime();
		float accTime  = m_pGM->GetAccTime();

		float fLerp = RoRMath::Lerp(0.f, 1.f, accTime / duration);

		m_vBlendColor.w = fLerp;
	}
	else if ((int)TutorialState::EndingOut == m_LVState)
	{
		float duration = m_pGM->GetEndingOutTime();
		float accTime  = m_pGM->GetAccTime();

		float fClamp = RoRMath::ClampFloat(accTime / duration, 0.f, 1.f);

		float fLerp = RoRMath::Lerp(0.f, 1.f, fClamp);

		m_vBlendColor.w = 1 - fLerp;
	}
}
