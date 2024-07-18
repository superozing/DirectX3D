#include "pch.h"
#include "CBossScript.h"

#include <Engine\CTimeMgr.h>
#include "CRoRStateMachine.h"

static string DebugState = "";

CBossScript::CBossScript()
	: CScript((UINT)SCRIPT_TYPE::BOSSSCRIPT)
	, m_BossStatus{}
	, m_GameState(BOSS_GAMESTATE::END)
	, m_AttDuration(0.f)
	, m_EXsDuration(0.f)
	, m_ActiveAttack(false)
	, m_ActiveEXs(false)
{
	AppendScriptParam("CurState", SCRIPT_PARAM::STRING, &DebugState);

	InitScriptParamUI();

	InitStateMachine();
}

CBossScript::~CBossScript()
{
	if (nullptr != m_FSM)
	{
		delete m_FSM;
		m_FSM = nullptr;
	}
}


void CBossScript::begin()
{
	SetBossGameState(BOSS_GAMESTATE::IDLE);

	m_FSM->Begin();
	m_FSM->SetCurState((int)BOSS_STATE::NormalIdle);
}

void CBossScript::tick()
{
	m_FSM->Update();
	DebugState = magic_enum::enum_name((BOSS_STATE)m_FSM->GetCurState());

	// 평타, 스킬 쿨타임 체크
	CheckDuration();

	// NormalAttack 상태
	CheckNormalAttack();

	// EXs 상태
	CheckEXs();

	// Vital 상태
	CheckVital();
}


void CBossScript::CheckDuration()
{
	if (!m_ActiveAttack)
	{
		m_AttDuration += DT;

		if (m_AttDuration >= m_BossStatus.ATTSpeed)
			m_ActiveAttack = true;
	}

	if (!m_ActiveEXs)
	{
		m_EXsDuration += DT;

		if (m_EXsDuration >= m_BossStatus.EXsCoolTime)
			m_ActiveEXs = true;
	}
}

void CBossScript::CheckNormalAttack()
{
	auto state = m_FSM->GetCurState();

	if ((int)BOSS_STATE::NormalIdle == state)
	{
		m_FSM->SetCurState((int)BOSS_STATE::NormalAttackStart);
		m_AttDuration = 0.f;
	}
}

void CBossScript::CheckEXs()
{
}

void CBossScript::CheckVital()
{
}


void CBossScript::InitStateMachine()
{
	m_FSM = new CRoRStateMachine<CBossScript>(this, (UINT)BOSS_STATE::END);

	FSMInit(BOSS_STATE, CBossScript, NormalIdle);
	FSMInit(BOSS_STATE, CBossScript, NormalAttackStart);
	FSMInit(BOSS_STATE, CBossScript, NormalAttackIng);
	FSMInit(BOSS_STATE, CBossScript, NormalAttackDelay);
	FSMInit(BOSS_STATE, CBossScript, NormalAttackEnd);
	FSMInit(BOSS_STATE, CBossScript, EXs1);
	FSMInit(BOSS_STATE, CBossScript, EXs2);
	FSMInit(BOSS_STATE, CBossScript, EXs3);
	FSMInit(BOSS_STATE, CBossScript, EXs4);
	FSMInit(BOSS_STATE, CBossScript, VitalDeath);
	FSMInit(BOSS_STATE, CBossScript, VitalGroggy);
	FSMInit(BOSS_STATE, CBossScript, VitalGroggyDeath);
	FSMInit(BOSS_STATE, CBossScript, Ready1);
	FSMInit(BOSS_STATE, CBossScript, Ready2);
}

void CBossScript::InitScriptParamUI()
{
	AppendScriptParam("GameState", SCRIPT_PARAM::STRING, &m_GameState);

	AppendScriptParam("IsDead", SCRIPT_PARAM::FLOAT, &m_BossStatus.IsDead);
	AppendScriptParam("MaxHP", SCRIPT_PARAM::FLOAT, &m_BossStatus.MaxHP, 0.f);
	AppendScriptParam("CurHP", SCRIPT_PARAM::FLOAT, &m_BossStatus.CurHP, 0.f);
	AppendScriptParam("AttackDamage", SCRIPT_PARAM::FLOAT, &m_BossStatus.ATTDamage, 0.f);
	AppendScriptParam("AttackSpeed", SCRIPT_PARAM::FLOAT, &m_BossStatus.ATTSpeed, 0.f);
	AppendScriptParam("EXsCoolTime", SCRIPT_PARAM::FLOAT, &m_BossStatus.EXsCoolTime, 0.f);
}
