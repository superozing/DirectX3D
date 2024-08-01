#include "pch.h"
#include "CDashEventListener.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CRoRStateMachine.h"
#include "CPlayerScript.h"
#include "CArona.h"
#include "CTutorialGameMode.h"

void CDashEventListener::begin()
{
	CEventListener::begin();

	m_pPlayerScript = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(PlayerName)->GetScript<CPlayerScript>();
	Activate(false);
	m_pArona = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(AronaName)->GetScript<CArona>();
}

void CDashEventListener::tick()
{
	CEventListener::tick();

	if (HasTargets())
	{
		Activate(true);
	}

	if (!IsActivate())
		return;

	if (!m_bFirstFrm)
	{
		m_pArona->Message("Press Space to Dash", 380, -1.f);
		m_bFirstFrm = true;
	}

	CEventListener::tick();

	auto pPlayerFSM = m_pPlayerScript->GetStateMachine();
	if (pPlayerFSM->StateBegin((int)PLAYER_STATE::SkillDash))
	{
		m_iDashCnt--;
	}

	if (m_iDashCnt <= 0)
	{
		m_pArona->Message("Congratulations!", 340, 3.f);

		CLevelMgr::GetInst()
			->GetCurrentLevel()
			->FindObjectByName(L"GameMode")
			->GetScript<CTutorialGameMode>()
			->Clear(TutorialState::Dash);

		GamePlayStatic::DestroyGameObject(GetOwner());
	}
}

CDashEventListener::CDashEventListener()
	: CEventListener((UINT)SCRIPT_TYPE::DASHEVENTLISTENER)
	, m_iDashCnt(3)
{
	AppendScriptParam("DashCnt", SCRIPT_PARAM::INT, &m_iDashCnt);
}

CDashEventListener::~CDashEventListener()
{
}
