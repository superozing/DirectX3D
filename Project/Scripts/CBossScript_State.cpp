#include "pch.h"
#include "CBossScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include "CRoRStateMachine.h"
#include "CBossBulletShellSpawner.h"
#include "CParticleSpawnScript.h"

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
	FireMegaFist();
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
	FireMiniGun();
}

int CBossScript::EXs1Update()
{
	if (!Animator3D()->IsPlayable())
		return (int)BOSS_STATE::NormalIdle;

	int idx = Animator3D()->GetCurFrameIdx();

	m_BulletInterval += DT;

	if (idx > 60 && idx < 123 && m_BulletInterval > 0.1f)
	{
		m_BulletShell->SpawnBossBulletShell(GetOwner(), 3.5f);
		m_BulletInterval = 0.f;
	}

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

	int idx = Animator3D()->GetCurFrameIdx();

	switch (idx)
	{
	case 167:
		if (!m_ArrMissile[0])
		{
			// 167번 프레임 1번 미사일 발사
			FireBossMissile(idx);
			m_ArrMissile[0] = true;
		}
		break;
	case 169:
		if (!m_ArrMissile[1])
		{
			// 169번 프레임 2번 미사일 발사
			FireBossMissile(idx);
			m_ArrMissile[1] = true;
		}
		break;
	case 173:
		if (!m_ArrMissile[2])
		{
			// 173번 프레임 3번 미사일 발사
			FireBossMissile(idx);
			m_ArrMissile[2] = true;
		}
		break;
	case 177:
		if (!m_ArrMissile[3])
		{
			// 177번 프레임 4번 미사일 발사
			FireBossMissile(idx);
			m_ArrMissile[3] = true;
		}
		break;
	case 180:
		if (!m_ArrMissile[4])
		{
			// 180번 프레임 5번 미사일 발사
			FireBossMissile(idx);
			m_ArrMissile[4] = true;
		}
		break;
	case 182:
		if (!m_ArrMissile[5])
		{
			// 182번 프레임 6번 미사일 발사
			FireBossMissile(idx);
			m_ArrMissile[5] = true;
		}
		break;
	case 186:
		if (!m_ArrMissile[6])
		{
			// 186번 프레임 7번 미사일 발사
			FireBossMissile(idx);
			m_ArrMissile[6] = true;
		}
		break;
	case 190:
		if (!m_ArrMissile[7])
		{
			// 190번 프레임 8번 미사일 발사
			FireBossMissile(idx);
			m_ArrMissile[7] = true;
		}
		break;
	default:
		break;
	}

	return m_FSM->GetCurState();
}

void CBossScript::EXs2End()
{
	for (int i = 0; i < 8; ++i)
	{
		m_ArrMissile[i] = false;
	}
}

void CBossScript::EXs3Begin()
{
	Animator3D()->Play((int)BOSS_STATE::EXs3, 0);

	ActiveHexShield();
}

int CBossScript::EXs3Update()
{
	if (!Animator3D()->IsPlayable())
		return (int)BOSS_STATE::NormalIdle;

	int idx = Animator3D()->GetCurFrameIdx();

	if (idx == 36 && !m_ArrShield[0])
	{
		ActiveInnerShield();
		m_ArrShield[0] = true;
	}

	if (idx == 46 && !m_ArrShield[1])
	{
		ActiveOutsideShield();
		m_ArrShield[1] = true;
	}

	return m_FSM->GetCurState();
}

void CBossScript::EXs3End()
{
	for (size_t i = 0; i < 2; ++i)
	{
		m_ArrShield[i] = false;
	}

	DeActiveHexShield();
}

void CBossScript::EXs4Begin()
{
	Animator3D()->Play((int)BOSS_STATE::EXs4, 0);

	m_ChaseDir = false;
	CheckTargetPos();
	ActiveWarningDecal();
	ActiveSwordTrail();
}

int CBossScript::EXs4Update()
{
	if (!Animator3D()->IsPlayable())
		return (int)BOSS_STATE::NormalIdle;

	int layeridx = Animator3D()->GetCurFrameIdx();

	if (layeridx > 120 && !m_bSwordBeam)
	{
		FireSwordBeam();
		m_bSwordBeam = true;

		CGameObject* pObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"SwordBeam_WarningDecal");
		if (nullptr != pObj)
			GamePlayStatic::DestroyGameObject(pObj);
	}

	return m_FSM->GetCurState();
}

void CBossScript::EXs4End()
{
	m_bSwordBeam = false;
	m_ChaseDir	 = true;

	if (IsVital())
	{
		CGameObject* pObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"SwordBeam_WarningDecal");
		if (nullptr != pObj)
			GamePlayStatic::DestroyGameObject(pObj);
	}
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
	m_ChaseDir = false;
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
	m_ChaseDir = false;

	// 그로기 전기 스파크 파티클 스폰
	Vec3 vPos = (Animator3D()->FindBoneMat(L"Bip001 Spine") * Transform()->GetWorldMat()).Translation();
	Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	vPos += vDir * 200.f;

	CGameObject* pObj	  = CAssetMgr::GetInst()->Load<CPrefab>(PREFp_Electric)->Instantiate();
	int			 layeridx = pObj->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(pObj, layeridx);
	pObj->GetScript<CParticleSpawnScript>()->SetParticleInfo(vPos, 7.8f);
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
	if (!m_BossStatus.IsDead)
	{
		m_BossStatus.IsGroggy = false;
		m_ChaseDir			  = true;
	}
}

void CBossScript::VitalGroggyDeathBegin()
{
	Animator3D()->Play((int)BOSS_STATE::VitalGroggyDeath, 0);
	m_ChaseDir = false;
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