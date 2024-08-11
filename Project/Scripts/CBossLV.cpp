#include "pch.h"
#include "CBossLV.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CRoRStateMachine.h"

static string state = "";

CBossLV::CBossLV()
	: CGameMode((UINT)SCRIPT_TYPE::BOSSLV)
	, m_OpeningInTime(0.5f)
	, m_OpeningDelayTime(1.f)
	, m_OpeningOutTime(0.5f)
	, m_PlayingInTime(3.f)
	, m_PlayingDelayTime(0.5f)
	, m_Acctime(0.f)
{
	m_FSM = new CRoRStateMachine<CBossLV>(this, (UINT)BossLV_STATE::END);

	FSMInit(BossLV_STATE, CBossLV, OpeningIn);
	FSMInit(BossLV_STATE, CBossLV, OpeningDelay);
	FSMInit(BossLV_STATE, CBossLV, OpeningOut);
	FSMInit(BossLV_STATE, CBossLV, PlayingIn);
	FSMInit(BossLV_STATE, CBossLV, PlayingDelay);
	FSMInit(BossLV_STATE, CBossLV, Playing);
	FSMInit(BossLV_STATE, CBossLV, PlayingOut);
	FSMInit(BossLV_STATE, CBossLV, Ending);

	AppendScriptParam("State", SCRIPT_PARAM::STRING, &state, 0.f, 0.f, true);
}

CBossLV::~CBossLV()
{
	if (m_FSM)
		delete m_FSM;
}

int CBossLV::GetCurLVState()
{
	return m_FSM->GetCurState();
}

void CBossLV::begin()
{
	m_FSM->Begin();
}

void CBossLV::tick()
{
	m_FSM->Update();
	state = magic_enum::enum_name((BossLV_STATE)m_FSM->GetCurState());
}

void CBossLV::OpeningInBegin()
{
}

int CBossLV::OpeningInUpdate()
{
	m_Acctime += DT;

	if (m_Acctime >= m_OpeningInTime)
	{
		return (int)BossLV_STATE::OpeningDelay;
	}

	return m_FSM->GetCurState();
}

void CBossLV::OpeningInEnd()
{
	m_Acctime = 0.f;
}

void CBossLV::OpeningDelayBegin()
{
}

int CBossLV::OpeningDelayUpdate()
{
	m_Acctime += DT;

	if (m_Acctime >= m_OpeningDelayTime)
	{
		return (int)BossLV_STATE::OpeningOut;
	}

	return m_FSM->GetCurState();
}

void CBossLV::OpeningDelayEnd()
{
	m_Acctime = 0.f;
}

void CBossLV::OpeningOutBegin()
{
}

int CBossLV::OpeningOutUpdate()
{
	m_Acctime += DT;

	if (m_Acctime >= m_OpeningOutTime)
	{
		return (int)BossLV_STATE::PlayingIn;
	}

	return m_FSM->GetCurState();
}

void CBossLV::OpeningOutEnd()
{
	m_Acctime = 0.f;
}

void CBossLV::PlayingInBegin()
{
}

int CBossLV::PlayingInUpdate()
{
	m_Acctime += DT;

	if (m_Acctime >= m_PlayingInTime)
	{
		return (int)BossLV_STATE::PlayingDelay;
	}

	return m_FSM->GetCurState();
}

void CBossLV::PlayingInEnd()
{
	m_Acctime = 0.f;
}

void CBossLV::PlayingDelayBegin()
{
}

int CBossLV::PlayingDelayUpdate()
{
	m_Acctime += DT;

	if (m_Acctime >= m_PlayingDelayTime)
	{
		return (int)BossLV_STATE::Playing;
	}

	return m_FSM->GetCurState();
}

void CBossLV::PlayingDelayEnd()
{
	m_Acctime = 0.f;
}

void CBossLV::PlayingBegin()
{
}

int CBossLV::PlayingUpdate()
{
	return m_FSM->GetCurState();
}

void CBossLV::PlayingEnd()
{
}

void CBossLV::PlayingOutBegin()
{
}

int CBossLV::PlayingOutUpdate()
{
	return m_FSM->GetCurState();
}

void CBossLV::PlayingOutEnd()
{
}

void CBossLV::EndingBegin()
{
}

int CBossLV::EndingUpdate()
{
	return m_FSM->GetCurState();
}

void CBossLV::EndingEnd()
{
}
