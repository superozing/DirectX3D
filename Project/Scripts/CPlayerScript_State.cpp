#include "pch.h"
#include "CPlayerScript.h"

#include "CRoRStateMachine.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CKeyMgr.h>

#include "CPlayerController.h"
#include "CCrosshair.h"
#include "CShootingSystemScript.h"

#define MoveStartCondition                                                              \
	(KEY_TAP(CPlayerController::Front) || KEY_PRESSED(CPlayerController::Front)) &&     \
		(KEY_TAP(CPlayerController::Back) || KEY_PRESSED(CPlayerController::Back)) &&   \
		(KEY_TAP(CPlayerController::Right) || KEY_PRESSED(CPlayerController::Right)) && \
		(KEY_TAP(CPlayerController::Left) || KEY_PRESSED(CPlayerController::Left))

#define MoveEndCondition                                                                  \
	(KEY_RELEASED(CPlayerController::Front) || KEY_NONE(CPlayerController::Front)) &&     \
		(KEY_RELEASED(CPlayerController::Back) || KEY_NONE(CPlayerController::Back)) &&   \
		(KEY_RELEASED(CPlayerController::Right) || KEY_NONE(CPlayerController::Right)) && \
		(KEY_RELEASED(CPlayerController::Left) || KEY_NONE(CPlayerController::Left))

#pragma region Normal

void CPlayerScript::NormalIdleBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::NormalIdle);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::NormalIdle]);
}

int CPlayerScript::NormalIdleUpdate()
{
	// TODO : 재장전 조건 추가 필요 (현재 탄창이 최대 탄창과 같으면 x)
	// 재장전
	if (KEY_TAP(CPlayerController::Reload))
		return (int)PLAYER_STATE::NormalReload;

	// 사격 준비
	if (KEY_TAP(CPlayerController::Zoom) || KEY_PRESSED(CPlayerController::Zoom))
		return (int)PLAYER_STATE::NormalAttackStart;

	return m_FSM->GetCurState();
}

void CPlayerScript::NormalIdleEnd()
{
}

void CPlayerScript::NormalReloadBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::NormalReload, 0);
}

int CPlayerScript::NormalReloadUpdate()
{
	// 애니메이션 종료시 Idle상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::NormalIdle;

	return m_FSM->GetCurState();
}

void CPlayerScript::NormalReloadEnd()
{
}

void CPlayerScript::NormalAttackStartBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::NormalAttackStart, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::NormalAttackStart]);
}

int CPlayerScript::NormalAttackStartUpdate()
{
	// 애니메이션 종료시 AttackDelay상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::NormalAttackDelay;

	return m_FSM->GetCurState();
}

void CPlayerScript::NormalAttackStartEnd()
{
}

void CPlayerScript::NormalAttackIngBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::NormalAttackIng, 0, 5.f);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::NormalAttackStart]);
	m_pShootingSystem->ShootPlayerBulletRay();
	m_pMuzzleFlash->GetOwner()->ParticleSystem()->Play();
}

int CPlayerScript::NormalAttackIngUpdate()
{
	// 애니메이션 종료시 AttackDelay상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::NormalAttackDelay;

	return m_FSM->GetCurState();
}

void CPlayerScript::NormalAttackIngEnd()
{
	m_pMuzzleFlash->GetOwner()->ParticleSystem()->Stop();
}

void CPlayerScript::NormalAttackDelayBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::NormalAttackDelay);
}

int CPlayerScript::NormalAttackDelayUpdate()
{
	if (KEY_TAP(CPlayerController::Reload))
		return (int)PLAYER_STATE::NormalReload;

	if (KEY_RELEASED(CPlayerController::Zoom) || KEY_NONE(CPlayerController::Zoom))
		return (int)PLAYER_STATE::NormalAttackEnd;

	return m_FSM->GetCurState();
}

void CPlayerScript::NormalAttackDelayEnd()
{
}

void CPlayerScript::NormalAttackEndBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::NormalAttackEnd, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::NormalIdle]);
}

int CPlayerScript::NormalAttackEndUpdate()
{
	// 애니메이션 종료시 Idle상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::NormalIdle;

	if (KEY_TAP(CPlayerController::Zoom))
		return (int)PLAYER_STATE::NormalAttackStart;

	return m_FSM->GetCurState();
}

void CPlayerScript::NormalAttackEndEnd()
{
}

#pragma endregion

#pragma region Stand

void CPlayerScript::StandIdleBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::StandIdle);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::StandIdle], 0.1f);
}

int CPlayerScript::StandIdleUpdate()
{
	// TODO : 재장전 조건 추가 필요 (현재 탄창이 최대 탄창과 같으면 x)
	// 재장전
	if (KEY_TAP(CPlayerController::Reload))
		return (int)PLAYER_STATE::StandReload;

	// 사격 준비
	if (KEY_TAP(CPlayerController::Zoom) || KEY_PRESSED(CPlayerController::Zoom))
		return (int)PLAYER_STATE::StandAttackStart;

	// 사격
	if ((KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack)) &&
		m_pShootingSystem->IsShootAvailable())
		return (int)PLAYER_STATE::StandAttackIng;

	return m_FSM->GetCurState();
}

void CPlayerScript::StandIdleEnd()
{
}

void CPlayerScript::StandReloadBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::StandReload, 0);
}

int CPlayerScript::StandReloadUpdate()
{
	// 애니메이션 종료시 Idle상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::StandIdle;

	return m_FSM->GetCurState();
}

void CPlayerScript::StandReloadEnd()
{
}

void CPlayerScript::StandAttackStartBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::StandAttackStart, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::StandAttackStart]);
}

int CPlayerScript::StandAttackStartUpdate()
{
	// 애니메이션 종료시 Attack Delay상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::StandAttackDelay;

	if ((KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack)) &&
		m_pShootingSystem->IsShootAvailable())
		return (int)PLAYER_STATE::StandAttackIng;

	return m_FSM->GetCurState();
}

void CPlayerScript::StandAttackStartEnd()
{
}

void CPlayerScript::StandAttackIngBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::StandAttackIng, 0, 5.f);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::StandAttackStart]);
	m_pShootingSystem->ShootPlayerBulletRay();
	m_pMuzzleFlash->GetOwner()->ParticleSystem()->Play();
}

int CPlayerScript::StandAttackIngUpdate()
{
	// 애니메이션 종료시 Attack Delay상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::StandAttackDelay;

	return m_FSM->GetCurState();
}

void CPlayerScript::StandAttackIngEnd()
{
	m_pMuzzleFlash->GetOwner()->ParticleSystem()->Stop();
}

void CPlayerScript::StandAttackDelayBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::StandAttackDelay);
}

int CPlayerScript::StandAttackDelayUpdate()
{
	// 사격
	if ((KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack)) &&
		m_pShootingSystem->IsShootAvailable())
		return (int)PLAYER_STATE::StandAttackIng;

	// 재장전
	if (KEY_TAP(CPlayerController::Reload))
		return (int)PLAYER_STATE::StandReload;

	// 사격 종료
	if (KEY_RELEASED(CPlayerController::Zoom) || KEY_NONE(CPlayerController::Zoom))
		return (int)PLAYER_STATE::StandAttackEnd;

	return m_FSM->GetCurState();
}

void CPlayerScript::StandAttackDelayEnd()
{
}

void CPlayerScript::StandAttackEndBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::StandAttackEnd, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::StandIdle]);
}

int CPlayerScript::StandAttackEndUpdate()
{
	// 애니메이션 종료시 Idle상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::StandIdle;

	// 사격 준비
	if (KEY_TAP(CPlayerController::Zoom))
		return (int)PLAYER_STATE::StandAttackStart;

	// 사격
	if ((KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack)) &&
		m_pShootingSystem->IsShootAvailable())
		return (int)PLAYER_STATE::StandAttackIng;

	return m_FSM->GetCurState();
}

void CPlayerScript::StandAttackEndEnd()
{
}

#pragma endregion

#pragma region Kneel

void CPlayerScript::KneelIdleBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::KneelIdle);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::KneelIdle], 0.1f);
}

int CPlayerScript::KneelIdleUpdate()
{
	// TODO : 재장전 조건 추가 필요 (현재 탄창이 최대 탄창과 같으면 x)
	// 재장전
	if (KEY_TAP(CPlayerController::Reload))
		return (int)PLAYER_STATE::KneelReload;

	// 사격 준비
	if (KEY_TAP(CPlayerController::Zoom) || KEY_PRESSED(CPlayerController::Zoom))
		return (int)PLAYER_STATE::KneelAttackStart;

	// 엄폐물 넘기
	if (KEY_TAP(CPlayerController::Jump))
		return (int)PLAYER_STATE::MoveJump;

	// 사격
	if ((KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack)) &&
		m_pShootingSystem->IsShootAvailable())
		return (int)PLAYER_STATE::KneelAttackIng;

	return m_FSM->GetCurState();
}

void CPlayerScript::KneelIdleEnd()
{
}

void CPlayerScript::KneelReloadBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::KneelReload, 0);
}

int CPlayerScript::KneelReloadUpdate()
{
	// 애니메이션 종료시 Idle상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::KneelIdle;

	return m_FSM->GetCurState();
}

void CPlayerScript::KneelReloadEnd()
{
}

void CPlayerScript::KneelAttackStartBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::KneelAttackStart, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::KneelAttackStart], 0.1f);
}

int CPlayerScript::KneelAttackStartUpdate()
{
	// 애니메이션 종료시 Attack Delay상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::KneelAttackDelay;

	// 사격
	if ((KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack)) &&
		m_pShootingSystem->IsShootAvailable())
		return (int)PLAYER_STATE::KneelAttackIng;

	return m_FSM->GetCurState();
}

void CPlayerScript::KneelAttackStartEnd()
{
}

void CPlayerScript::KneelAttackIngBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::KneelAttackIng, 0, 5.f);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::KneelAttackStart], 0.1f);
	m_pShootingSystem->ShootPlayerBulletRay();
	m_pMuzzleFlash->GetOwner()->ParticleSystem()->Play();
}

int CPlayerScript::KneelAttackIngUpdate()
{
	// 애니메이션 종료시 Attack Delay상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::KneelAttackDelay;

	return m_FSM->GetCurState();
}

void CPlayerScript::KneelAttackIngEnd()
{
	m_pMuzzleFlash->GetOwner()->ParticleSystem()->Stop();
}

void CPlayerScript::KneelAttackDelayBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::KneelAttackDelay);
}

int CPlayerScript::KneelAttackDelayUpdate()
{
	// 사격
	if ((KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack)) &&
		m_pShootingSystem->IsShootAvailable())
		return (int)PLAYER_STATE::KneelAttackIng;

	// 재장전
	if (KEY_TAP(CPlayerController::Reload))
		return (int)PLAYER_STATE::KneelReload;

	// 사격 준비
	if (KEY_RELEASED(CPlayerController::Zoom) || KEY_NONE(CPlayerController::Zoom))
		return (int)PLAYER_STATE::KneelAttackEnd;

	return m_FSM->GetCurState();
}

void CPlayerScript::KneelAttackDelayEnd()
{
}

void CPlayerScript::KneelAttackEndBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::KneelAttackEnd, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::KneelIdle], 0.1f);
}

int CPlayerScript::KneelAttackEndUpdate()
{
	// 애니메이션 종료시 Idle상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::KneelIdle;

	// 사격 준비
	if (KEY_TAP(CPlayerController::Zoom))
		return (int)PLAYER_STATE::KneelAttackStart;

	// 사격
	if ((KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack)) &&
		m_pShootingSystem->IsShootAvailable())
		return (int)PLAYER_STATE::KneelAttackIng;

	return m_FSM->GetCurState();
}

void CPlayerScript::KneelAttackEndEnd()
{
}

#pragma endregion

#pragma region Move

void CPlayerScript::MoveStartNormalBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveStartNormal, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::NormalIdle]);
}

int CPlayerScript::MoveStartNormalUpdate()
{
	// 움직임 종료
	if (MoveEndCondition)
		return (int)PLAYER_STATE::MoveEndNormal;

	// 애니메이션 종료시 MoveIng상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::MoveIng;

	return m_FSM->GetCurState();
}

void CPlayerScript::MoveStartNormalEnd()
{
}

void CPlayerScript::MoveStartStandBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveStartStand, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::NormalIdle]);
}

int CPlayerScript::MoveStartStandUpdate()
{
	// 움직임 종료
	if (MoveEndCondition)
		return (int)PLAYER_STATE::MoveEndStand;

	// 애니메이션 종료시 MoveIng상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::MoveIng;

	return m_FSM->GetCurState();
}

void CPlayerScript::MoveStartStandEnd()
{
}

void CPlayerScript::MoveStartKneelBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveStartKneel, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::NormalIdle]);
}

int CPlayerScript::MoveStartKneelUpdate()
{
	// 움직임 종료
	if (MoveEndCondition)
		return (int)PLAYER_STATE::MoveEndKneel;

	// 애니메이션 종료시 MoveIng상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::MoveIng;

	return m_FSM->GetCurState();
}

void CPlayerScript::MoveStartKneelEnd()
{
}

void CPlayerScript::MoveEndNormalBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveEndNormal, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::NormalIdle]);
}

int CPlayerScript::MoveEndNormalUpdate()
{
	// 애니메이션 종료시 Idle상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::NormalIdle;

	// 움직임종료상태 움직임 속도 조절 - 좌우 움직일때 어색함으로 삭제
	//{
	//	int maxFrm = Animator3D()->GetCurClipLength();
	//	int curFrm = Animator3D()->GetCurFrameIdx();

	//	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	//	vFront.Normalize();
	//	Vec3 vPos = Transform()->GetRelativePos();

	//	float fSpeed = RoRMath::SmoothStep(m_tStatus.DashMinSpeed, 0, (float)curFrm / maxFrm);

	//	vPos += vFront * fSpeed * DT;

	//	Transform()->SetRelativePos(vPos);
	//}

	// 사격 준비
	if (KEY_TAP(CPlayerController::Zoom) || KEY_PRESSED(CPlayerController::Zoom))
		return (int)PLAYER_STATE::NormalAttackStart;

	return m_FSM->GetCurState();
}

void CPlayerScript::MoveEndNormalEnd()
{
}

void CPlayerScript::MoveEndStandBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveEndStand, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::StandIdle]);
}

int CPlayerScript::MoveEndStandUpdate()
{
	// 애니메이션 종료시 Idle상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::StandIdle;
	// 사격 준비
	if (KEY_TAP(CPlayerController::Zoom) || KEY_PRESSED(CPlayerController::Zoom))
		return (int)PLAYER_STATE::StandAttackStart;

	// 사격
	if ((KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack)) &&
		m_pShootingSystem->IsShootAvailable())
		return (int)PLAYER_STATE::StandAttackIng;

	return m_FSM->GetCurState();
}

void CPlayerScript::MoveEndStandEnd()
{
}

void CPlayerScript::MoveEndKneelBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveEndKneel, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::KneelIdle]);
}

int CPlayerScript::MoveEndKneelUpdate()
{
	// 애니메이션 종료시 Idle상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::KneelIdle;

	// 사격 준비
	if (KEY_TAP(CPlayerController::Zoom) || KEY_PRESSED(CPlayerController::Zoom))
		return (int)PLAYER_STATE::KneelAttackStart;

	// 사격
	if ((KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack)) &&
		m_pShootingSystem->IsShootAvailable())
		return (int)PLAYER_STATE::KneelAttackIng;

	return m_FSM->GetCurState();
}

void CPlayerScript::MoveEndKneelEnd()
{
}

void CPlayerScript::MoveIngBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveIng);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::NormalIdle]);
}

int CPlayerScript::MoveIngUpdate()
{
	if (MoveEndCondition)
	{
		switch (GetCoverType())
		{
		case CoverType::Normal:
			return (int)PLAYER_STATE::MoveEndNormal;
			break;
		case CoverType::Stand:
			return (int)PLAYER_STATE::MoveEndStand;
			break;
		case CoverType::Kneel:
			return (int)PLAYER_STATE::MoveEndKneel;
			break;
		}
	}

	// 사격 준비
	if (KEY_TAP(CPlayerController::Zoom))
		return (int)PLAYER_STATE::NormalAttackStart;

	// TODO: Reload 조건
	if (KEY_TAP(CPlayerController::Reload))
		return (int)PLAYER_STATE::NormalReload;

	return m_FSM->GetCurState();
}

void CPlayerScript::MoveIngEnd()
{
}

void CPlayerScript::MoveJumpBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveJump, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::MoveJump]);
	m_fJumpY = Transform()->GetRelativePos().y;
}

int CPlayerScript::MoveJumpUpdate()
{
	// 애니메이션 종료시 Idle상태로 전환
	if (!Animator3D()->IsPlayable())
	{
		return MoveStartCondition ? (int)PLAYER_STATE::MoveIng : (int)PLAYER_STATE::MoveEndNormal;
	}

	Vec3 vPos	= Transform()->GetRelativePos();
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	vPos += vFront * m_tStatus.JumpSpeed * DT;

	int curFrm = Animator3D()->GetCurFrameIdx();
	int maxFrm = Animator3D()->GetCurClipLength();

	float x = (float)curFrm / maxFrm;
	// float y		= -(16.f / 9.f) * pow((x - 2.f / 5.f), 2.f) + 1;
	float y		= -4.f * pow((x - 1.f / 2.f), 2.f) + 1;
	float scale = 50.f;
	vPos.y		= m_fJumpY + y * scale;

	Transform()->SetRelativePos(vPos);

	return m_FSM->GetCurState();
}

void CPlayerScript::MoveJumpEnd()
{
	Vec3 vPos = Transform()->GetRelativePos();
	vPos.y	  = m_fJumpY;
	Transform()->SetRelativePos(vPos);
}

#pragma endregion

#pragma region Vital

void CPlayerScript::VitalDeathBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::VitalDeath, 0);
}

int CPlayerScript::VitalDeathUpdate()
{
	return m_FSM->GetCurState();
}

void CPlayerScript::VitalDeathEnd()
{
}

void CPlayerScript::VitalPanicBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::VitalPanic, 0);
}

int CPlayerScript::VitalPanicUpdate()
{
	// TODO: 패닉이 풀렸는지 검사해야 함
	// return ispanic ? (int)PLAYER_STATE::NormalIdle : m_FSM->GetCurState();
	return m_FSM->GetCurState();
}

void CPlayerScript::VitalPanicEnd()
{
}

void CPlayerScript::VitalDyingBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::VitalDying, 0);
}

int CPlayerScript::VitalDyingUpdate()
{
	return m_FSM->GetCurState();
}

void CPlayerScript::VitalDyingEnd()
{
}

#pragma endregion

#pragma region Victory

void CPlayerScript::VictoryStartBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::VictoryStart, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::VictoryStart]);
}

int CPlayerScript::VictoryStartUpdate()
{
	// 애니메이션 종료시 Victory End상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::VictoryEnd;

	return m_FSM->GetCurState();
}

void CPlayerScript::VictoryStartEnd()
{
}

void CPlayerScript::VictoryEndBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::VictoryEnd, 0);
}

int CPlayerScript::VictoryEndUpdate()
{
	// 애니메이션 종료시 Idle상태로 전환
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::NormalIdle;

	return m_FSM->GetCurState();
}

void CPlayerScript::VictoryEndEnd()
{
}

#pragma endregion

#pragma region Skill

void CPlayerScript::SkillDashBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::SkillDash, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::SkillDash]);
	SetInvincivility(true);
}

int CPlayerScript::SkillDashUpdate()
{
	// 대시상태 움직임 속도 조절
	int curFrm = Animator3D()->GetCurFrameIdx();
	int maxFrm = Animator3D()->GetCurClipLength();
	if (curFrm > maxFrm)
		return SwitchToCoverTypeIdle();

	float fCurSpeed;
	int	  iGroudnTapFrm = 20;

	// 땅에 닿는 순간을 분기점으로 스피드 변경 + 무적 판정 해제
	if (curFrm < iGroudnTapFrm)
	{
		fCurSpeed =
			RoRMath::SmoothStep(m_tStatus.DashMaxSpeed, m_tStatus.DashGroundSpeed, (float)curFrm / iGroudnTapFrm);
	}
	else
	{
		fCurSpeed = RoRMath::SmoothStep(m_tStatus.DashGroundSpeed, m_tStatus.DashMinSpeed,
										(float)(curFrm - iGroudnTapFrm) / (maxFrm - iGroudnTapFrm));
		SetInvincivility(false);
	}

	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	vFront.Normalize();
	Vec3 vPos = Transform()->GetRelativePos();
	vPos += vFront * fCurSpeed * DT;

	Transform()->SetRelativePos(vPos);

	// 애니메이션 종료시 MoveEnd상태로 전환
	int stopFrm = 60;
	if (curFrm > stopFrm)
		return SwitchToCoverTypeMoveEnd();

	return m_FSM->GetCurState();
}

void CPlayerScript::SkillDashEnd()
{
}

static bool bThrow = false;

void CPlayerScript::SkillThrowBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::SkillThrow, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::SkillThrow]);
	bThrow = false;
}

int CPlayerScript::SkillThrowUpdate()
{
	int curFrm = Animator3D()->GetCurFrameIdx();

	// 애니메이션 종료시 Idle상태로 전환
	// 현재 애니메이션 길이가 좀 길어서 50이상이면 끝냄
	if (curFrm >= 50)
		return SwitchToCoverTypeIdle();

	if (!bThrow && curFrm == 12)
	{
		Animator3D()->Pause(true);
		if (KEY_TAP(CPlayerController::Skill))
		{
			Animator3D()->Pause(false);
			bThrow = true;
		}
	}

	return m_FSM->GetCurState();
}

void CPlayerScript::SkillThrowEnd()
{
}

void CPlayerScript::SkillCallsignBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::SkillCallsign, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::SkillCallsign]);
}

int CPlayerScript::SkillCallsignUpdate()
{
	// 애니메이션 종료시 Idle상태로 전환
	if (!Animator3D()->IsPlayable())
		return SwitchToCoverTypeIdle();

	return m_FSM->GetCurState();
}

void CPlayerScript::SkillCallsignEnd()
{
}

static bool bEXFirst  = false;
static bool bEXSecond = false;
static bool bEXThird  = false;

void CPlayerScript::SkillEXBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::SkillEX, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::SkillEX]);
	bEXFirst  = false;
	bEXSecond = false;
	bEXThird  = false;
}

int CPlayerScript::SkillEXUpdate()
{
	// 애니메이션 종료시 Idle상태로 전환
	if (!Animator3D()->IsPlayable())
		return SwitchToCoverTypeIdle();

	int curFrm = Animator3D()->GetCurFrameIdx();

	auto info = m_mSpringInfos[PLAYER_STATE::SkillEX];

	if (!bEXFirst && curFrm <= 10)
	{
	}
	else if (!bEXFirst && curFrm >= 20)
	{
		bEXFirst = true;
		// CTimeMgr::GetInst()->SetDTScale(0.1f);
		info.fMaxDistance = 70.f;
		info.fCamSpeed	  = 10.f;
		info.fCamRotSpeed = 20.f;
		info.vDir		  = Vec3(0.f, 180.f, 0.f);
		// info.vOffsetPos	  = Vec2(50.f, 70.f);
		m_pSpringArm->SetInfo(info);
		Animator3D()->SetPlaybackSpeed(0.1f);
	}
	else if (!bEXSecond && curFrm >= 28)
	{
		// CTimeMgr::GetInst()->SetDTScale(1.f);
		Animator3D()->SetPlaybackSpeed(1.f);
		bEXSecond = true;
		// info.fMaxDistance = 100.f;
		// info.fCamSpeed	  = 10.f;
		// info.vDir		  = Vec3(-20.f, 30.f, 0.f);
		// info.vOffsetPos	  = Vec2(0.f, 50.f);
		info.fCamSpeed	  = 10.f;
		info.fCamRotSpeed = 20.f;
		m_pSpringArm->SetInfo(info);
	}
	else if (!bEXThird && curFrm >= 45)
	{
		Animator3D()->SetPlaybackSpeed(1.f);
		bEXThird = true;
		// info.fMaxDistance = 200.f;
		info.fCamSpeed	= 10.f;
		info.vDir		= Vec3(-20.f, 30.f, 0.f);
		info.vOffsetPos = Vec2(0.f, 50.f);
		m_pSpringArm->SetInfo(info);
	}
	else if (curFrm >= 75)
	{
		info.fMaxDistance = 250.f;
		info.fCamSpeed	  = 10.f;
		m_pSpringArm->SetInfo(info);
	}

	return m_FSM->GetCurState();
}

void CPlayerScript::SkillEXEnd()
{
}

#pragma endregion

#pragma region Formation

void CPlayerScript::FormationIdleBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::FormationIdle, 0);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::FormationIdle]);
}

int CPlayerScript::FormationIdleUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::NormalIdle;

	return m_FSM->GetCurState();
}

void CPlayerScript::FormationIdleEnd()
{
}

#pragma endregion

// int CPlayerScript::AttackUpdate()
//{
//	// 기본 움직임
//	CGameObject* pCamera = CRenderMgr::GetInst()->GetMainCam()->GetOwner();
//
//	Vec3 vPos	= Transform()->GetRelativePos();
//	Vec3 vFront = pCamera->Transform()->GetWorldDir(DIR_TYPE::FRONT);
//	Vec3 vRight = pCamera->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
//
//	if (KEY_PRESSED(W))
//	{
//		vPos += vFront * m_tStatus.AttackMoveSpeed * DT;
//	}
//	if (KEY_PRESSED(S))
//	{
//		vPos += -vFront * m_tStatus.AttackMoveSpeed * DT;
//	}
//	if (KEY_PRESSED(D))
//	{
//		vPos += vRight * m_tStatus.AttackMoveSpeed * DT;
//	}
//	if (KEY_PRESSED(A))
//	{
//		vPos += -vRight * m_tStatus.AttackMoveSpeed * DT;
//	}
//
//	Transform()->SetRelativePos(vPos);
//
//	if (KEY_TAP(KEY::SPACE))
//	{
//		return m_FSM->GetCurState() + 1;
//	}
//	return m_FSM->GetCurState();
// }
