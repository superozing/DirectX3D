#include "pch.h"
#include "CTutVictoryTex.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include "CTutorialGameMode.h"

CTutVictoryTex::CTutVictoryTex()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::TUTVICTORYTEX)
	, m_pGM(nullptr)
	, m_LVState((int)TutorialState::END)
	, m_EndingIn(false)
	, m_EndingOut(false)
{
}

CTutVictoryTex::~CTutVictoryTex()
{
}

void CTutVictoryTex::begin()
{
	CAtlasImageUIScript::begin();

	m_UIImg = CAssetMgr::GetInst()->Load<CTexture>(TEXCampaign_Imgfont_Victory);
	SetAtlasSize(Vec2(0.f, 0.f), Vec2(m_UIImg->GetWidth(), m_UIImg->GetHeight()));

	m_pGM = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"GameMode")->GetScript<CTutorialGameMode>();

	Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
}

void CTutVictoryTex::tick()
{
	CAtlasImageUIScript::tick();

	m_LVState = m_pGM->GetCurState();

	if ((int)TutorialState::EndingDelay == m_LVState && !m_EndingIn)
	{
		Transform()->Lerp(Transform()->GetRelativePos(), false, Transform()->GetRelativeRotation(), true,
						  Vec3(1024.f, 256.f, 0.f), 0.1f);

		m_EndingIn = true;
	}
	else if ((int)TutorialState::EndingOut == m_LVState && !m_EndingOut)
	{
		Transform()->Lerp(Transform()->GetRelativePos(), false, Transform()->GetRelativeRotation(), true,
						  Vec3(0.f, 0.f, 0.f), 0.1f);

		m_EndingOut = true;
	}
}
