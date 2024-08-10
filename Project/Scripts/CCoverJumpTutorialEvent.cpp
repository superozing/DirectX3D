#include "pch.h"
#include "CCoverJumpTutorialEvent.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CPlayerScript.h"
#include "CArona.h"
#include "CTutorialGameMode.h"

CCoverJumpTutorialEvent::CCoverJumpTutorialEvent()
	: CEventListener((UINT)SCRIPT_TYPE::COVERJUMPTUTORIALEVENT)
{
}

CCoverJumpTutorialEvent::~CCoverJumpTutorialEvent()
{
}

void CCoverJumpTutorialEvent::Start()
{
	// if (HasTargets() && m_pGM->IsClear(TutorialState::CoverHigh))
	//{
	//	tick_func = &CCoverJumpTutorialEvent::EventClear;
	//	m_pArona->Message("Cover And Press Space Bar To Jump", 600);
	// }
}

void CCoverJumpTutorialEvent::EventClear()
{
	if (m_pPlayerScript->GetStateMachine()->StateBegin((int)PLAYER_STATE::MoveJump))
	{
		GamePlayStatic::DestroyGameObject(GetOwner());
		m_pArona->Message("Congratulations!", 340, 3.f);
		// m_pGM->Clear(TutorialState::CoverJump);
	}
}

void CCoverJumpTutorialEvent::begin()
{
	CEventListener::begin();
	CLevel* pLevel	= CLevelMgr::GetInst()->GetCurrentLevel();
	m_pPlayerScript = pLevel->FindObjectByName(PlayerName)->GetScript<CPlayerScript>();
	m_pArona		= pLevel->FindObjectByName(AronaName)->GetScript<CArona>();
	m_pGM			= pLevel->FindObjectByName(TutGMName)->GetScript<CTutorialGameMode>();

	tick_func = &CCoverJumpTutorialEvent::Start;
}

void CCoverJumpTutorialEvent::tick()
{
	CEventListener::tick();

	(this->*tick_func)();
}
