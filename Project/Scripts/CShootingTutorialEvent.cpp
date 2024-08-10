#include "pch.h"
#include "CShootingTutorialEvent.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>

#include "CArona.h"
#include "CPlayerScript.h"
#include "CTutorialGameMode.h"
#include "CSpawnSpotScript.h"
#include "CTutorialTarget.h"

static bool	 bSecondMsg = false;
static float AccTime	= 0.f;

CShootingTutorialEvent::CShootingTutorialEvent()
	: CEventListener((UINT)SCRIPT_TYPE::SHOOTINGTUTORIALEVENT)
{
	for (int i = 0; i < SPAWNERCNT; i++)
	{
		string msg = "Target" + to_string(i + 1);
		AppendScriptParam(msg, SCRIPT_PARAM::BOOL, &m_vecClear[i]);
	}
}

CShootingTutorialEvent::~CShootingTutorialEvent()
{
}

void CShootingTutorialEvent::begin()
{
	CEventListener::begin();

	auto pLevel		= CLevelMgr::GetInst()->GetCurrentLevel();
	m_pArona		= pLevel->FindObjectByName(AronaName)->GetScript<CArona>();
	m_pPlayerScript = pLevel->FindObjectByName(PlayerName)->GetScript<CPlayerScript>();
	m_pGM			= pLevel->FindObjectByName(TutGMName)->GetScript<CTutorialGameMode>();
	for (int i = 0; i < SPAWNERCNT; i++)
	{
		wstring name = L"TargetSpawner" + to_wstring(i + 1);
		m_vecTargets.push_back(pLevel->FindObjectByName(name)->GetScript<CSpawnSpotScript>());
		m_vecTargets[i]->RegisterObject();
		m_vecClear[i] = false;
	}
	m_pWall = pLevel->FindObjectByName(L"WALL_SHOOT");

	Activate(false);

	m_bFirstFrm = false;
	bSecondMsg	= false;
	AccTime		= 0.f;
}

void CShootingTutorialEvent::tick()
{
	CEventListener::tick();

	// if (HasTargets() && m_pGM->IsClear(TutorialState::Dash))
	Activate(true);

	if (!IsActivate())
		return;

	float MsgDelay = 3.f;
	if (!m_bFirstFrm)
	{
		m_pArona->Message("Right Mouse Button Is Zoom And  Left Button Is Shoot", 860, MsgDelay);
		m_bFirstFrm = true;
	}

	float SpawnDelay = 1.f;
	if (!bSecondMsg)
	{
		AccTime += DT;
		if (AccTime >= SpawnDelay)
		{
			m_pArona->Message("Remove all targets", 360, -1.f);
			bSecondMsg = true;
			for (size_t i = 0; i < SPAWNERCNT; i++)
			{
				m_vecTargetScripts.push_back(m_vecTargets[i]->SpawnObject()->GetScript<CTutorialTarget>());
			}
		}
		return;
	}

	bool bClear = true;
	for (size_t i = 0; i < SPAWNERCNT; i++)
	{
		bClear &= m_vecClear[i] = m_vecTargetScripts[i]->IsHit();
	}

	if (bClear)
	{
		// m_pGM->Clear(TutorialState::Shooting);
		GamePlayStatic::DestroyGameObject(GetOwner());
	}
}
