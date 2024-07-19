#include "pch.h"
#include "CBossScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CRandomMgr.h>
#include "CRoRStateMachine.h"

static string DebugState = "";

CBossScript::CBossScript()
	: CScript((UINT)SCRIPT_TYPE::BOSSSCRIPT)
	, m_BossStatus{}
	, m_AttDuration(0.f)
	, m_EXsDuration(0.f)
	, m_ActiveAttack(false)
	, m_ActiveEXs(false)
	, m_EXsType(0)
{
	AppendScriptParam("CurState    ", SCRIPT_PARAM::STRING, &DebugState);

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
	m_FSM->Begin();
	m_FSM->SetCurState((int)BOSS_STATE::NormalIdle);
}

void CBossScript::tick()
{
	m_FSM->Update();
	DebugState = magic_enum::enum_name((BOSS_STATE)m_FSM->GetCurState());

	if ((int)BOSS_STATE::NormalIdle == m_FSM->GetCurState())
	{
		// 평타, 스킬 쿨타임 체크
		CheckDuration();
	}
	//// NormalAttack 상태
	// CheckNormalAttack();

	//// EXs 상태
	// CheckEXs();

	// Vital 상태
	CheckVital();
}

void CBossScript::CheckDuration()
{
	if (!m_ActiveAttack)
	{
		m_AttDuration += DT;

		if (m_AttDuration >= m_BossStatus.ATTSpeed)
		{
			m_AttDuration  = 0.f;
			m_ActiveAttack = true;
		}
	}

	if (!m_ActiveEXs)
	{
		m_EXsDuration += DT;

		if (m_EXsDuration >= m_BossStatus.EXsCoolTime)
		{
			m_EXsDuration = 0.f;
			m_ActiveEXs	  = true;
		}
	}

	if (m_ActiveEXs)
	{
		// m_EXsType = CRandomMgr::GetInst()->GetRandomInt(4);

		switch (m_EXsType)
		{
		case 0:
			m_FSM->SetCurState((int)BOSS_STATE::EXs1);
			break;
		case 1:
			m_FSM->SetCurState((int)BOSS_STATE::EXs2);
			break;
		case 2:
			m_FSM->SetCurState((int)BOSS_STATE::EXs3);
			break;
		case 3:
			m_FSM->SetCurState((int)BOSS_STATE::EXs4);
			break;
		default:
			break;
		}
		m_ActiveEXs = false;
	}
	else if (m_ActiveAttack)
	{
		m_FSM->SetCurState((int)BOSS_STATE::NormalAttackStart);
		m_ActiveAttack = false;
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
	if (m_BossStatus.CurHP <= 0.f)
		m_BossStatus.IsDead = true;

	if (m_BossStatus.IsGroggy && !m_BossStatus.IsDead)
		m_FSM->SetCurState((int)BOSS_STATE::VitalGroggy);
	else if (!m_BossStatus.IsGroggy && m_BossStatus.IsDead)
		m_FSM->SetCurState((int)BOSS_STATE::VitalDeath);
	else if (m_BossStatus.IsGroggy && m_BossStatus.IsDead)
		m_FSM->SetCurState((int)BOSS_STATE::VitalGroggyDeath);
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
	AppendScriptParam("MaxHP       ", SCRIPT_PARAM::FLOAT, &m_BossStatus.MaxHP, 0.f);
	AppendScriptParam("CurHP       ", SCRIPT_PARAM::FLOAT, &m_BossStatus.CurHP, 0.f);
	AppendScriptParam("AttackDamage", SCRIPT_PARAM::FLOAT, &m_BossStatus.ATTDamage, 0.f);
	AppendScriptParam("AttackSpeed ", SCRIPT_PARAM::FLOAT, &m_BossStatus.ATTSpeed, 0.f);
	AppendScriptParam("EXsCoolTime ", SCRIPT_PARAM::FLOAT, &m_BossStatus.EXsCoolTime, 0.f);
	AppendScriptParam("EXsType     ", SCRIPT_PARAM::INT, &m_EXsType, 0, 3);

	AppendScriptParam("Dead        ", SCRIPT_PARAM::BOOL, &m_BossStatus.IsDead);
	AppendScriptParam("Groggy      ", SCRIPT_PARAM::BOOL, &m_BossStatus.IsGroggy);
}
