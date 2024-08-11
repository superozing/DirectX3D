#include "pch.h"
#include "CTurret.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLogMgr.h>

#include "CRoRStateMachine.h"
#include "CParticleSpawnScript.h"
#include "CPlayerScript.h"

void CTurret::InitStateMachine()
{
	m_FSM = new CRoRStateMachine<CTurret>(this, (UINT)TURRET_STATE::END);

	FSMInit(TURRET_STATE, CTurret, Appear);
	FSMInit(TURRET_STATE, CTurret, NormalIdle);
	FSMInit(TURRET_STATE, CTurret, Reload);
	FSMInit(TURRET_STATE, CTurret, AttackStart);
	FSMInit(TURRET_STATE, CTurret, AttackDelay);
	FSMInit(TURRET_STATE, CTurret, Attack);
	FSMInit(TURRET_STATE, CTurret, AttackEnd);
	FSMInit(TURRET_STATE, CTurret, VitalRetreat);
	FSMInit(TURRET_STATE, CTurret, VitalPanic);
	FSMInit(TURRET_STATE, CTurret, VitalDeath);
}

int CTurret::GetCurState()
{
	return m_FSM->GetCurState();
}

void CTurret::AppearBegin()
{
	Animator3D()->Play((int)TURRET_STATE::Appear, 0);
}

int CTurret::AppearUpdate()
{

	if (!Animator3D()->IsPlayable())
		return ((int)TURRET_STATE::NormalIdle);

	return m_FSM->GetCurState();
}

void CTurret::AppearEnd()
{
}

void CTurret::VitalPanicBegin()
{
}

int CTurret::VitalPanicUpdate()
{
	return 0;
}

void CTurret::VitalPanicEnd()
{
}

void CTurret::VitalDeathBegin()
{
}

int CTurret::VitalDeathUpdate()
{
	return 0;
}

void CTurret::VitalDeathEnd()
{
}

void CTurret::SwitchNormal()
{
}

void CTurret::SwitchPanic()
{
}

void CTurret::SwitchDead()
{
}

void CTurret::NormalIdleBegin()
{
	Animator3D()->Play((int)TURRET_STATE::NormalIdle);
}

int CTurret::NormalIdleUpdate()
{
	return m_FSM->GetCurState();
}

void CTurret::NormalIdleEnd()
{
}

void CTurret::ReloadBegin()
{
}

int CTurret::ReloadUpdate()
{
	return 0;
}

void CTurret::ReloadEnd()
{
}

void CTurret::AttackStartBegin()
{
	Animator3D()->Play((int)TURRET_STATE::AttackStart, 0);
}

int CTurret::AttackStartUpdate()
{
	if (!Animator3D()->IsPlayable())
		return ((int)TURRET_STATE::Attack);

	return m_FSM->GetCurState();
}

void CTurret::AttackStartEnd()
{
}

void CTurret::AttackDelayBegin()
{
}

int CTurret::AttackDelayUpdate()
{
	return 0;
}

void CTurret::AttackDelayEnd()
{
}

void CTurret::AttackBegin()
{
	Animator3D()->Play((int)TURRET_STATE::Attack, 0);
}

int CTurret::AttackUpdate()
{
	if (!Animator3D()->IsPlayable())
		return ((int)TURRET_STATE::AttackEnd);

	return m_FSM->GetCurState();
}

void CTurret::AttackEnd()
{
}

void CTurret::AttackEndBegin()
{
	Animator3D()->Play((int)TURRET_STATE::AttackEnd, 0);
}

int CTurret::AttackEndUpdate()
{
	if (!Animator3D()->IsPlayable())
		return ((int)TURRET_STATE::NormalIdle);

	return m_FSM->GetCurState();
}

void CTurret::AttackEndEnd()
{
	fAttackTimer = AttackTimer;
}

void CTurret::VitalRetreatBegin()
{
}

int CTurret::VitalRetreatUpdate()
{
	return 0;
}

void CTurret::VitalRetreatEnd()
{
}
