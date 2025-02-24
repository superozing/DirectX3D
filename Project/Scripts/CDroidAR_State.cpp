﻿#include "pch.h"
#include "CDroidAR.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLogMgr.h>
#include <Engine\CRandomMgr.h>

#include "CRoRStateMachine.h"

#include "CParticleSpawnScript.h"
#include "CShootingSystem_DroidAR.h"
#include "CMuzzleFlash_DroidAR.h"
#include "CPlayerScript.h"

void CDroidAR::AppereBegin()
{
	Animator3D()->Play((int)DROIDAR_STATE::Appere, 0);
	m_vecSound[(UINT)DroidARSoundType::Spawn]->Play(1, 4.f, false);
}

int CDroidAR::AppereUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)DROIDAR_STATE::MoveIng;

	return m_FSM->GetCurState();
}

void CDroidAR::AppereEnd()
{
}

void CDroidAR::EXsBegin()
{
	Animator3D()->Play((int)DROIDAR_STATE::EXs, 0);
}

int CDroidAR::EXsUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)DROIDAR_STATE::NormalIdle;

	return m_FSM->GetCurState();
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
	// 만약 업데이트 중에 다른 곳을 가고싶다면?

	// 예를 들어 노말 아이들 이 끝나자마자 attack으로 가고 싶다면?
	// if (!Animator3D()->IsPlayable()) // -> 애니메이션이 끝난 상태.
	//	return (int)DROIDAR_STATE::NormalAttackStart; // 가고싶은 state로 반환하기.
	// 또는 SetCurState()를 사용해서 스테이트를 바꿀 수 있다.

	// 시점
	// 어떤 애니메이션에서 어떤 애니메이션(state)로 흘러갈 것인가? 에 대한 고민이 필요함. 이야...
	// if (hp가 0이면) SetCurState()로 상태를 바꿔준다거나..

	// return -> 업데이트 할 때 다음 바뀔 스테이트가 무엇인지 지정
	// set() -> 진짜 아무곳에서나 바꿀 수 있음. begin이랑 end에서는 호출하지 않아야 함.
	// 체력이 바뀌었을 때 바로 dead 상태로 바꾸어야 할 정도로 바로 보내야 하는 경우.

	return m_FSM->GetCurState();
}

void CDroidAR::NormalIdleEnd()
{
}

void CDroidAR::NormalReloadBegin()
{
	// 장전 시켜주기
	m_AttackCount = m_MaxAttackCount;
	Animator3D()->Play((int)DROIDAR_STATE::NormalReload, 0);
}

int CDroidAR::NormalReloadUpdate()
{
	// 애니메이션 종료 시 idle 상태에서 attack state 전환 기다리기
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
	// 조준 애니메이션
	if (!Animator3D()->IsPlayable())
		return (int)DROIDAR_STATE::NormalAttackIng;

	return m_FSM->GetCurState();
}

void CDroidAR::NormalAttackStartEnd()
{
	m_ShootingSystem->SetShootingDirection(m_Target->GetScript<CPlayerScript>());
	RotationDirectionToPlayer();
}

void CDroidAR::NormalAttackIngBegin()
{
	Animator3D()->Play((int)DROIDAR_STATE::NormalAttackIng, 0);
	--m_AttackCount;
	m_ShootingSystem->ShootDroidARBulletRay();
	m_MuzzleFlash->ParticleSystem()->Play();
	m_vecSound[(UINT)DroidARSoundType::Attack]->Play(1, 0.6f, true);
}

int CDroidAR::NormalAttackIngUpdate()
{
	// 애니메이션 재생이 끝났을 경우
	if (!Animator3D()->IsPlayable())
	{
		// 탄창이 비었나요?
		if (m_AttackCount <= 0)
			return (int)DROIDAR_STATE::NormalAttackEnd;
		// 탄창이 비지 않았나요?
		else
			return (int)DROIDAR_STATE::NormalAttackDelay;
	}

	return m_FSM->GetCurState();
}

void CDroidAR::NormalAttackIngEnd()
{
	m_MuzzleFlash->ParticleSystem()->Stop();
	// 1. particle
}

void CDroidAR::NormalAttackDelayBegin()
{
	Animator3D()->Play((int)DROIDAR_STATE::NormalAttackDelay, 0);
}

int CDroidAR::NormalAttackDelayUpdate()
{
	// 일정 시간 대기 후 다시 총 쏘러 가요.
	if (!Animator3D()->IsPlayable())
		return (int)DROIDAR_STATE::NormalAttackIng;

	Vec3 PlayerPos = m_Target->Transform()->GetWorldPos();
	Vec3 DroidPos  = GetOwner()->Transform()->GetWorldPos();

	Vec3 dir = PlayerPos - DroidPos;
	dir.y	 = 0.f;
	GetOwner()->Transform()->SetDir(dir.Normalize());

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
		return (int)DROIDAR_STATE::NormalReload;

	return m_FSM->GetCurState();
}

void CDroidAR::NormalAttackEndEnd()
{
}

void CDroidAR::MoveIngBegin()
{
	Animator3D()->Play((int)DROIDAR_STATE::MoveIng);
}

int CDroidAR::MoveIngUpdate()
{
	if (Animator3D()->GetCurFrameIdx() == 0 && m_IsLeftFootSound)
	{
		m_vecSound[(UINT)PlayerSoundType::MOVEMENT]->Play(1, 3.f, true);
		m_IsLeftFootSound = false;
	}
	if (Animator3D()->GetCurFrameIdx() == Animator3D()->GetCurClipLength() / 2 && !m_IsLeftFootSound)
	{
		m_vecSound[(UINT)PlayerSoundType::MOVEMENT]->Play(1, 3.f, true);
		m_IsLeftFootSound = true;
	}

	if (CheckTargetInRange())
		return (int)DROIDAR_STATE::MoveEnd;

	RotationDirectionToPlayer();

	return m_FSM->GetCurState();
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
	if (!Animator3D()->IsPlayable())
		return (int)DROIDAR_STATE::NormalIdle;

	return m_FSM->GetCurState();
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
	m_vecSound[(UINT)DroidARSoundType::Death]->Play(1, 1.f, true);
	if (PhysX())
		GetOwner()->DeleteComponent(COMPONENT_TYPE::PHYSX);
}

int CDroidAR::VitalDeathUpdate()
{
	int curFrm = Animator3D()->GetCurFrameIdx();
	int maxFrm = Animator3D()->GetCurClipLength();

	if (curFrm == maxFrm - 2)
	{
		Animator3D()->Pause(true);
		m_DeathAfterTimer -= DT;

		if (m_DeathAfterTimer <= 0.f)
			GamePlayStatic::DestroyGameObject(this->GetOwner());
	}

	return m_FSM->GetCurState();
}

void CDroidAR::VitalDeathEnd()
{
}
