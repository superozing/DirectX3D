#include "pch.h"
#include "CTutorialEndingEvent.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CPlayerScript.h"
#include "CTutorialGameMode.h"
#include "CArona.h"

void CTutorialEndingEvent::Start()
{
	if (HasTargets() && m_pGM->IsClear(TutorialState::CoverLow))
	{
		m_pArona->Message("Congratulations! You All Clear Tutorial", 650, 3.f);
	}
}

void CTutorialEndingEvent::ClearEvent()
{
}

void CTutorialEndingEvent::begin()
{
	CEventListener::begin();

	CLevel* pLevel	= CLevelMgr::GetInst()->GetCurrentLevel();
	m_pPlayerScript = pLevel->FindObjectByName(PlayerName)->GetScript<CPlayerScript>();
	m_pArona		= pLevel->FindObjectByName(AronaName)->GetScript<CArona>();
	m_pGM			= pLevel->FindObjectByName(TutGMName)->GetScript<CTutorialGameMode>();

	tick_func = &CTutorialEndingEvent::Start;
}

void CTutorialEndingEvent::tick()
{
	CEventListener::tick();

	(this->*tick_func)();
}

CTutorialEndingEvent::CTutorialEndingEvent()
	: CEventListener((UINT)SCRIPT_TYPE::TUTORIALENDINGEVENT)
{
}

CTutorialEndingEvent::~CTutorialEndingEvent()
{
}
