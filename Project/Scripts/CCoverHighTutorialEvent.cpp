#include "pch.h"
#include "CCoverHIghTutorialEvent.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CArona.h"
#include "CTutorialGameMode.h"
#include "CPlayerScript.h"

CCoverHIghTutorialEvent::CCoverHIghTutorialEvent()
	: CEventListener((UINT)SCRIPT_TYPE::COVERHIGHTUTORIALEVENT)
{
}

CCoverHIghTutorialEvent::~CCoverHIghTutorialEvent()
{
}

void CCoverHIghTutorialEvent::Start()
{
	// if (HasTargets() && m_pGM->IsClear(TutorialState::Shooting))
	//{
	//	m_pArona->Message("Go to the cover & Press LShift to cover it up", 700.f);
	//	tick_func = &CCoverHIghTutorialEvent::Cover;
	// }
}

void CCoverHIghTutorialEvent::Cover()
{
	if (m_pPlayerScript->GetCoverType() != CoverType::Normal)
	{
		m_pArona->Message("Terminate Enemy", 350.f);
		tick_func = &CCoverHIghTutorialEvent::Combat;
	}
}

void CCoverHIghTutorialEvent::Combat()
{
	// if (m_pMonster1->isDead() && m_pMonster2->IsDead())
	//{
	//	tick_func = nullptr;
	// }

	GamePlayStatic::DestroyGameObject(GetOwner());
	m_pArona->Message("Congratulations!", 340, 3.f);
	// m_pGM->Clear(TutorialState::CoverHigh);
}

void CCoverHIghTutorialEvent::begin()
{
	CEventListener::begin();

	CLevel* pLevel	= CLevelMgr::GetInst()->GetCurrentLevel();
	m_pPlayerScript = pLevel->FindObjectByName(PlayerName)->GetScript<CPlayerScript>();
	m_pArona		= pLevel->FindObjectByName(AronaName)->GetScript<CArona>();
	m_pGM			= pLevel->FindObjectByName(TutGMName)->GetScript<CTutorialGameMode>();

	// TODO : 몬스터 생기면 주석풀어야 함
	// m_pMonster1 = pLevel->FindObjectByName(L"Enemy1")->GetScript<CMonster>();
	// m_pMonster2 = pLevel->FindObjectByName(L"Enemy2")->GetScript<CMonster>();
	tick_func = &CCoverHIghTutorialEvent::Start;
}

void CCoverHIghTutorialEvent::tick()
{
	(this->*tick_func)();
}
