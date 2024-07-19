#include "pch.h"
#include "CBossScript.h"

#include "CRoRStateMachine.h"

#pragma region Normal

void CBossScript::NormalIdleBegin()
{
	Animator3D()->Play((int)BOSS_STATE::NormalIdle);
}

int CBossScript::NormalIdleUpdate()
{
	return m_FSM->GetCurState();
}

void CBossScript::NormalIdleEnd()
{
}

void CBossScript::NormalReloadBegin()
{
	Animator3D()->Play((int)BOSS_STATE::NormalReload, 0);
}

int CBossScript::NormalReloadUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)BOSS_STATE::NormalIdle;

	return m_FSM->GetCurState();
}

void CBossScript::NormalReloadEnd()
{
}

void CBossScript::NormalAttackStartBegin()
{
	Animator3D()->Play((int)BOSS_STATE::NormalAttackStart, 0);
}

int CBossScript::NormalAttackStartUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)BOSS_STATE::NormalAttackIng;

	return m_FSM->GetCurState();
}

void CBossScript::NormalAttackStartEnd()
{
}

void CBossScript::NormalAttackIngBegin()
{
	Animator3D()->Play((int)BOSS_STATE::NormalAttackIng, 0);
}

int CBossScript::NormalAttackIngUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)BOSS_STATE::NormalAttackEnd;

	return m_FSM->GetCurState();
}

void CBossScript::NormalAttackIngEnd()
{
}

void CBossScript::NormalAttackDelayBegin()
{
	Animator3D()->Play((int)BOSS_STATE::NormalAttackDelay, 0);
}

int CBossScript::NormalAttackDelayUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)BOSS_STATE::NormalAttackEnd;

	return m_FSM->GetCurState();
}

void CBossScript::NormalAttackDelayEnd()
{
}

void CBossScript::NormalAttackEndBegin()
{
	Animator3D()->Play((int)BOSS_STATE::NormalAttackEnd, 0);
}

int CBossScript::NormalAttackEndUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)BOSS_STATE::NormalIdle;

	return m_FSM->GetCurState();
}

void CBossScript::NormalAttackEndEnd()
{
}

#pragma endregion

#pragma region EXs

void CBossScript::EXs1Begin()
{
	Animator3D()->Play((int)BOSS_STATE::EXs1, 0);
}

int CBossScript::EXs1Update()
{
	if (!Animator3D()->IsPlayable())
		return (int)BOSS_STATE::NormalIdle;

	return m_FSM->GetCurState();
}

void CBossScript::EXs1End()
{
}

void CBossScript::EXs2Begin()
{
	Animator3D()->Play((int)BOSS_STATE::EXs2, 0);
}

int CBossScript::EXs2Update()
{
	if (!Animator3D()->IsPlayable())
		return (int)BOSS_STATE::NormalIdle;

	return m_FSM->GetCurState();
}

void CBossScript::EXs2End()
{
}

void CBossScript::EXs3Begin()
{
	Animator3D()->Play((int)BOSS_STATE::EXs3, 0);
}

int CBossScript::EXs3Update()
{
	if (!Animator3D()->IsPlayable())
		return (int)BOSS_STATE::NormalIdle;

	return m_FSM->GetCurState();
}

void CBossScript::EXs3End()
{
}

void CBossScript::EXs4Begin()
{
	Animator3D()->Play((int)BOSS_STATE::EXs4, 0);
}

int CBossScript::EXs4Update()
{
	if (!Animator3D()->IsPlayable())
		return (int)BOSS_STATE::NormalIdle;

	return m_FSM->GetCurState();
}

void CBossScript::EXs4End()
{
}

void CBossScript::EXs5Begin()
{
	Animator3D()->Play((int)BOSS_STATE::EXs5, 0);
}

int CBossScript::EXs5Update()
{
	if (!Animator3D()->IsPlayable())
		return (int)BOSS_STATE::NormalIdle;

	return m_FSM->GetCurState();
}

void CBossScript::EXs5End()
{
}

#pragma endregion

#pragma region Vital

void CBossScript::VitalDeathBegin()
{
	Animator3D()->Play((int)BOSS_STATE::VitalDeath, 0);
}

int CBossScript::VitalDeathUpdate()
{
	return m_FSM->GetCurState();
}

void CBossScript::VitalDeathEnd()
{
}

void CBossScript::VitalGroggyBegin()
{
	Animator3D()->Play((int)BOSS_STATE::VitalGroggy, 0);
}

int CBossScript::VitalGroggyUpdate()
{
	if (!m_BossStatus.IsDead && !Animator3D()->IsPlayable())
		return (int)BOSS_STATE::NormalIdle;
	else if (m_BossStatus.IsDead)
		return (int)BOSS_STATE::VitalGroggyDeath;

	return m_FSM->GetCurState();
}

void CBossScript::VitalGroggyEnd()
{
}

void CBossScript::VitalGroggyDeathBegin()
{
	Animator3D()->Play((int)BOSS_STATE::VitalGroggyDeath, 0);
}

int CBossScript::VitalGroggyDeathUpdate()
{
	return m_FSM->GetCurState();
}

void CBossScript::VitalGroggyDeathEnd()
{
}

#pragma endregion

#pragma region BattleReady

void CBossScript::Ready1Begin()
{
	Animator3D()->Play((int)BOSS_STATE::Ready1, 0);
}

int CBossScript::Ready1Update()
{
	if (!Animator3D()->IsPlayable())
		return (int)BOSS_STATE::Ready2;

	return m_FSM->GetCurState();
}

void CBossScript::Ready1End()
{
}

void CBossScript::Ready2Begin()
{
	Animator3D()->Play((int)BOSS_STATE::Ready2, 0);
}

int CBossScript::Ready2Update()
{
	if (!Animator3D()->IsPlayable())
		return (int)BOSS_STATE::NormalIdle;

	return m_FSM->GetCurState();
}

void CBossScript::Ready2End()
{
}

#pragma endregion