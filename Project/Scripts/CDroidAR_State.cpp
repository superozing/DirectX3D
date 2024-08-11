#include "pch.h"
#include "CDroidAR.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLogMgr.h>
#include <Engine\CRandomMgr.h>

#include "CRoRStateMachine.h"

#include "CParticleSpawnScript.h"
#include "CPlayerScript.h"

void CDroidAR::AppereBegin()
{
}

int CDroidAR::AppereUpdate()
{
	return 0;
}

void CDroidAR::AppereEnd()
{
}

void CDroidAR::EXsBegin()
{
}

int CDroidAR::EXsUpdate()
{
	return 0;
}

void CDroidAR::EXsEnd()
{
}

void CDroidAR::NormalIdleBegin()
{
	Animator3D()->Play((int)DROIDAR_STATE::NormalIdle);
}

int CDroidAR::NormalIdleUpdate()
{
	return m_FSM->GetCurState();
}

void CDroidAR::NormalIdleEnd()
{
}

void CDroidAR::NormalReloadBegin()
{
	Animator3D()->Play((int)DROIDAR_STATE::NormalReload, 0);
}


int CDroidAR::NormalReloadUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)DROIDAR_STATE::NormalIdle;

	return m_FSM->GetCurState();
}

void CDroidAR::NormalReloadEnd()
{
}

void CDroidAR::NormalAttackStartBegin()
{
	Animator3D()->Play((int)DROIDAR_STATE::NormalAttackStart, 0);
}

int CDroidAR::NormalAttackStartUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)DROIDAR_STATE::NormalAttackIng;

	return m_FSM->GetCurState();
}

void CDroidAR::NormalAttackStartEnd()
{
	CDroidAR::Fire();
}

void CDroidAR::NormalAttackIngBegin()
{
	Animator3D()->Play((int)DROIDAR_STATE::NormalAttackStart, 0);
}

int CDroidAR::NormalAttackIngUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)DROIDAR_STATE::NormalAttackEnd;

	return m_FSM->GetCurState();
}

void CDroidAR::NormalAttackIngEnd()
{
}

void CDroidAR::NormalAttackDelayBegin()
{
	Animator3D()->Play((int)DROIDAR_STATE::NormalAttackDelay, 0);
}

int CDroidAR::NormalAttackDelayUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)DROIDAR_STATE::NormalAttackEnd;

	return m_FSM->GetCurState();
}

void CDroidAR::NormalAttackDelayEnd()
{
}

void CDroidAR::NormalAttackEndBegin()
{
	Animator3D()->Play((int)DROIDAR_STATE::NormalAttackEnd, 0);
}

int CDroidAR::NormalAttackEndUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)DROIDAR_STATE::NormalIdle;

	return m_FSM->GetCurState();
}

void CDroidAR::NormalAttackEndEnd()
{
}

void CDroidAR::MoveIngBegin()
{
	Animator3D()->Play((int)DROIDAR_STATE::MoveIng, 0);
}

int CDroidAR::MoveIngUpdate()
{
	return 0;
}

void CDroidAR::MoveIngEnd()
{
}

void CDroidAR::MoveEndBegin()
{
	Animator3D()->Play((int)DROIDAR_STATE::MoveEnd, 0);
}

int CDroidAR::MoveEndUpdate()
{
	return 0;
}

void CDroidAR::MoveEndEnd()
{
}

void CDroidAR::VitalPanicBegin()
{
	Animator3D()->Play((int)DROIDAR_STATE::VitalPanic, 0);
}

int CDroidAR::VitalPanicUpdate()
{
	return 0;
}

void CDroidAR::VitalPanicEnd()
{
}

void CDroidAR::VitalDeathBegin()
{
	Animator3D()->Play((int)DROIDAR_STATE::VitalDeath, 0);
}

int CDroidAR::VitalDeathUpdate()
{
	return m_FSM->GetCurState();
}

void CDroidAR::VitalDeathEnd()
{
}

