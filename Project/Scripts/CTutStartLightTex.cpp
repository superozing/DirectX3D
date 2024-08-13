#include "pch.h"
#include "CTutStartLightTex.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include "CTutorialGameMode.h"

CTutStartLightTex::CTutStartLightTex()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::TUTSTARTLIGHTTEX)
	, m_pGM(nullptr)
	, m_LVState((int)TutorialState::END)
{
}

CTutStartLightTex::~CTutStartLightTex()
{
}

void CTutStartLightTex::begin()
{
	CAtlasImageUIScript::begin();

	m_UIImg = CAssetMgr::GetInst()->Load<CTexture>(TEXIngame_Start_Light);
	SetAtlasSize(Vec2(0.f, 0.f), Vec2(m_UIImg->GetWidth(), m_UIImg->GetHeight()));
	m_vBlendColor.w = 0.f;

	m_pGM = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"GameMode")->GetScript<CTutorialGameMode>();
}

void CTutStartLightTex::tick()
{
	CAtlasImageUIScript::tick();

	m_LVState = m_pGM->GetCurState();

	if ((int)TutorialState::OpeningIn == m_LVState)
	{
		float duration = m_pGM->GetOpeningInTime();
		float accTime  = m_pGM->GetAccTime();

		float fLerp = RoRMath::Lerp(0.f, 1.f, accTime / duration);

		m_vBlendColor.w = fLerp;
	}
	else if ((int)TutorialState::OpeningOut == m_LVState)
	{
		m_vBlendColor.w = 0.f;
	}
}
