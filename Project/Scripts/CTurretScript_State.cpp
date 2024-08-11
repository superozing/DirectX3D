#include "pch.h"
#include "CTurret.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLogMgr.h>

#include "CRoRStateMachine.h"
#include "CParticleSpawnScript.h"
#include "CPlayerScript.h"

#include "CProjectileScript.h"

static float DeathAfterTimer = 0.5f;

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

void CTurret::RotateIdle()
{
	if (m_FSM->GetCurState() == (int)TURRET_STATE::NormalIdle && m_Target != nullptr)
	{
		Vec3 PlayerPos = m_Target->Transform()->GetWorldPos();
		Vec3 TurretPos = GetOwner()->Transform()->GetWorldPos();

		Vec3 dir		= (PlayerPos - TurretPos).Normalize();
		Vec3 currentDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);

		float t		 = m_tStatus.RotateSpeed * DT; // 회전 속도에 따른 보간 계수
		Vec3  newDir = RoRMath::Lerp(currentDir, dir, t);

		// 새로운 방향으로 터렛을 회전시킵니다.
		Transform()->SetDir(newDir);
	}
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
	Animator3D()->Play((int)TURRET_STATE::VitalDeath, 0);

	Vec3 CurPos = Transform()->GetWorldPos();

	CGameObject* pObj	  = CAssetMgr::GetInst()->Load<CPrefab>(PREFp_Explode_Lite)->Instantiate();
	int			 layeridx = pObj->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(pObj, layeridx);
	pObj->GetScript<CParticleSpawnScript>()->SetParticleInfo(CurPos, 1.5f);

	pObj	 = CAssetMgr::GetInst()->Load<CPrefab>(PREFp_Explode)->Instantiate();
	layeridx = pObj->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(pObj, layeridx);
	pObj->GetScript<CParticleSpawnScript>()->SetParticleInfo(CurPos, 1.f);
}

int CTurret::VitalDeathUpdate()
{
	int curFrm = Animator3D()->GetCurFrameIdx();
	int maxFrm = Animator3D()->GetCurClipLength();

	if (curFrm == maxFrm - 2)
	{
		Animator3D()->Pause(true);
		DeathAfterTimer -= DT;

		if (DeathAfterTimer <= 0.f)
			GamePlayStatic::DestroyGameObject(this->GetOwner());
	}

	return m_FSM->GetCurState();
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
