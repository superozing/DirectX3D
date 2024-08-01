#include "pch.h"
#include "CShootingTutorialEvent.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>

#include "CArona.h"
#include "CPlayerScript.h"
#include "CTutorialGameMode.h"

void CShootingTutorialEvent::begin()
{
	CEventListener::begin();

	auto pLevel		= CLevelMgr::GetInst()->GetCurrentLevel();
	m_pArona		= pLevel->FindObjectByName(AronaName)->GetScript<CArona>();
	m_pPlayerScript = pLevel->FindObjectByName(PlayerName)->GetScript<CPlayerScript>();
	m_pGM			= pLevel->FindObjectByName(TutGMName)->GetScript<CTutorialGameMode>();
	m_pWall			= pLevel->FindObjectByName(L"WALL_SHOOT");

	Activate(false);
}

void CShootingTutorialEvent::tick()
{
	CEventListener::tick();

	if (HasTargets())
		Activate(true);

	if (!IsActivate() || !m_pGM->IsClear(TutorialState::Dash))
		return;

	float MsgDelay = 3.f;
	if (!m_bFirstFrm)
	{
		m_pArona->Message("Right Mouse Button Is Zoom And  Left Button Is Shoot", 860, MsgDelay);
		m_bFirstFrm = true;
	}

	static bool bSecondMsg = false;
	if (!bSecondMsg)
	{
		static float AccTime = 0.f;
		AccTime += DT;
		if (AccTime >= MsgDelay)
		{
			m_pArona->Message("Remove all targets", 360, -1.f);
			bSecondMsg = true;
		}
	}
}

CShootingTutorialEvent::CShootingTutorialEvent()
	: CEventListener((UINT)SCRIPT_TYPE::SHOOTINGTUTORIALEVENT)
{
}

CShootingTutorialEvent::~CShootingTutorialEvent()
{
}
