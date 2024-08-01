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

	if (HasTargets() && m_pGM->IsClear(TutorialState::Dash))
		Activate(true);

	if (!IsActivate())
		return;

	float MsgDelay = 3.f;
	if (!m_bFirstFrm)
	{
		m_pArona->Message("Right Mouse Button Is Zoom And  Left Button Is Shoot", 860, MsgDelay);
		m_bFirstFrm = true;
	}

	if (!bSecondMsg)
	{
		AccTime += DT;
		if (AccTime >= MsgDelay)
		{
			m_pArona->Message("Remove all targets", 360, -1.f);
			bSecondMsg = true;
			for (size_t i = 0; i < SPAWNERCNT; i++)
			{
				m_vecTargetScripts.push_back(m_vecTargets[i]->SpawnObject()->GetScript<CTutorialTarget>());
			}

			// 타겟 오브젝트가 몬스터 스크립트를 상속해서 제대로 된 적중을 판단할 수 있다면 축하메시지 출력 타이밍으로
			// 옮겨주어야 함
			Vec3 vPos	= m_pWall->Transform()->GetRelativePos();
			Vec3 vScale = m_pWall->Transform()->GetRelativeScale();
			vPos.x += vScale.z;
			m_pWall->Transform()->Lerp(vPos, false, Vec3(), false, Vec3(), 2.f);
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
		m_pArona->Message("Congratulations!", 340, 3.f);
		m_pGM->Clear(TutorialState::Shooting);
		GamePlayStatic::DestroyGameObject(GetOwner());
	}
}
