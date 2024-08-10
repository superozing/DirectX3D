#include "pch.h"
#include "CCoverLowEventListener.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include "CPlayerScript.h"
#include "CArona.h"
#include "CTutorialGameMode.h"

CCoverLowEventListener::CCoverLowEventListener()
	: CEventListener((UINT)SCRIPT_TYPE::COVERLOWEVENTLISTENER)
{
}

CCoverLowEventListener::~CCoverLowEventListener()
{
}

void CCoverLowEventListener::Start()
{
	// if (HasTargets() && m_pGM->IsClear(TutorialState::CoverJump))
	//{
	//	m_pArona->Message("Go to the cover high & Press LShift to cover it up", 700.f);
	//	tick_func = &CCoverLowEventListener::ClearEvent;
	// }
}

void CCoverLowEventListener::ClearEvent()
{
	// if (m_pMonster1->IsDead() && m_pMonster2->IsDead())
	//{
	// }

	GamePlayStatic::DestroyGameObject(GetOwner());
	m_pArona->Message("Congratulations!", 340, 3.f);
	// m_pGM->Clear(TutorialState::CoverLow);
}

void CCoverLowEventListener::begin()
{
	CEventListener::begin();

	CLevel* pLevel	= CLevelMgr::GetInst()->GetCurrentLevel();
	m_pPlayerScript = pLevel->FindObjectByName(PlayerName)->GetScript<CPlayerScript>();
	m_pArona		= pLevel->FindObjectByName(AronaName)->GetScript<CArona>();
	m_pGM			= pLevel->FindObjectByName(TutGMName)->GetScript<CTutorialGameMode>();

	tick_func = &CCoverLowEventListener::Start;
}

void CCoverLowEventListener::tick()
{
	CEventListener::tick();
	(this->*tick_func)();
}