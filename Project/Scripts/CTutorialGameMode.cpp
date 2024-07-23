#include "pch.h"
#include "CTutorialGameMode.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CEventListener.h"

static string state = "";

CTutorialGameMode::CTutorialGameMode()
	: CGameMode((UINT)SCRIPT_TYPE::TUTORIALGAMEMODE)
{
	m_FSM = new CRoRStateMachine<CTutorialGameMode>(this, (UINT)TutorialState::END);

	FSMInit(TutorialState, CTutorialGameMode, BasicMove);
	FSMInit(TutorialState, CTutorialGameMode, Dash);
	FSMInit(TutorialState, CTutorialGameMode, Shooting);
	FSMInit(TutorialState, CTutorialGameMode, CombatFirst);
	FSMInit(TutorialState, CTutorialGameMode, CombatSecond);

	AppendScriptParam("State", SCRIPT_PARAM::STRING, &state, 0.f, 0.f, true);
}

CTutorialGameMode::~CTutorialGameMode()
{
	if (m_FSM)
		delete m_FSM;
}

void CTutorialGameMode::begin()
{
	m_FSM->Begin();
}

void CTutorialGameMode::tick()
{
	m_FSM->Update();
	state = magic_enum::enum_name((TutorialState)m_FSM->GetCurState());
}

void CTutorialGameMode::BasicMoveBegin()
{
}

int CTutorialGameMode::BasicMoveUpdate()
{
	// TODO : 충분한 기본적인 움직임(WASD), 마우스 움직임 검사하고 dasheventlistner 활성화 시켜줘야 함
	if (ClearBasicMove())
	{
		return m_FSM->GetCurState() + 1;
	}

	return m_FSM->GetCurState();
}

void CTutorialGameMode::BasicMoveEnd()
{
}

void CTutorialGameMode::DashBegin()
{
}

int CTutorialGameMode::DashUpdate()
{
	// TODO : 끝에 도달하면 수고했다는 메시지랑 다음 길 안내정도 스크립트 띄워주면 좋을듯
	if (ClearDash())
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CTutorialGameMode::DashEnd()
{
}

void CTutorialGameMode::ShootingBegin()
{
}

int CTutorialGameMode::ShootingUpdate()
{
	// TODO : 서서쏴 하는 곳
	// 1. 바닥 2개 생성
	// 2. 공중 2개 생성
	// 3. 바닥, 공중 2개 생성

	if (ClearShooting())
	{
		return m_FSM->GetCurState() + 1;
	}

	return m_FSM->GetCurState();
}

void CTutorialGameMode::ShootingEnd()
{
}

void CTutorialGameMode::CombatFirstBegin()
{
}

int CTutorialGameMode::CombatFirstUpdate()
{
	// TODO : 소환된 몬스터를 모두 잡으면
	// 1. 위치에 도달하면 몬스터 생성
	// 2. 몬스터한테 맞으면 다음단계
	// 3. 스탠드 엄폐 하면 다음단계
	// 4. 줌하면 다음단계
	// 5. 사격하면 다음단계
	// 6. 모두 잡으면 다음단계

	if (ClearCombatFirst())
	{
		return m_FSM->GetCurState() + 1;
	}

	return m_FSM->GetCurState();
}

void CTutorialGameMode::CombatFirstEnd()
{
}

void CTutorialGameMode::CombatSecondBegin()
{
}

int CTutorialGameMode::CombatSecondUpdate()
{
	// TODO : 세분화
	// 1. 일정 지점에 도달하면 몬스터 소환
	// 2. 낮은 엄폐에 도달해서 맞으면 다음단계
	// 3. 낮은 엄폐하면 다음 단계
	// 4. 점프하면 다음단계
	// 5. 낮은엄폐하고 사격하면 다음단계
	// 6. 수류탄 던지면 다음단계
	// 6. 모두 잡으면 다음단계

	if (ClearCombatSecond())
	{
		return m_FSM->GetCurState() + 1;
	}

	return m_FSM->GetCurState();
}

void CTutorialGameMode::CombatSecondEnd()
{
}

bool CTutorialGameMode::ClearBasicMove()
{
	m_arrIsClear[(UINT)TutorialState::BasicMove] = true;
	return false;
}

void CTutorialGameMode::ChangeToDash()
{
	if (m_arrIsClear[m_FSM->GetCurState()])
		m_FSM->SetCurState((int)TutorialState::Dash);
}

bool CTutorialGameMode::ClearDash()
{
	return false;
}

void CTutorialGameMode::ChangeToShooting()
{
	if (m_arrIsClear[m_FSM->GetCurState()])
		m_FSM->SetCurState((int)TutorialState::Shooting);
}

bool CTutorialGameMode::ClearShooting()
{
	m_arrIsClear[(UINT)TutorialState::Shooting] = true;
	return false;
}

void CTutorialGameMode::ChangeToCombatFirst()
{
	if (m_arrIsClear[m_FSM->GetCurState()])
		m_FSM->SetCurState((int)TutorialState::CombatFirst);
}

bool CTutorialGameMode::ClearCombatFirst()
{
	m_arrIsClear[(UINT)TutorialState::CombatFirst] = true;
	return false;
}

void CTutorialGameMode::ChangeToCombatSecond()
{
	if (m_arrIsClear[m_FSM->GetCurState()])
		m_FSM->SetCurState((int)TutorialState::CombatSecond);
}

bool CTutorialGameMode::ClearCombatSecond()
{
	m_arrIsClear[(UINT)TutorialState::CombatSecond] = true;
	return false;
}
