#include "pch.h"
#include "CPlayerScript.h"

#include "CRoRStateMachine.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CKeyMgr.h>

#include "CPlayerController.h"

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
	{
		return (int)PLAYER_STATE::NormalReload;
	}

	// 사격 준비
	if (KEY_TAP(CPlayerController::Zoom) || KEY_PRESSED(CPlayerController::Zoom))
	{
		return (int)PLAYER_STATE::NormalAttackStart;
	}

	// 스킬
	if (KEY_TAP(CPlayerController::Dash))
	{
		return (int)PLAYER_STATE::SkillDash;
	}

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
}

int CPlayerScript::NormalAttackIngUpdate()
{
	if (!Animator3D()->IsPlayable())
	{
		return (int)PLAYER_STATE::NormalAttackDelay;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::NormalAttackIngEnd()
{
}

void CPlayerScript::NormalAttackDelayBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::NormalAttackDelay);
}

int CPlayerScript::NormalAttackDelayUpdate()
{
	if (KEY_TAP(CPlayerController::Reload))
	{
		return (int)PLAYER_STATE::NormalReload;
	}

	if (KEY_RELEASED(CPlayerController::Zoom) || KEY_NONE(CPlayerController::Zoom))
	{
		return (int)PLAYER_STATE::NormalAttackEnd;
	}
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
	// TODO: 상하 에임 구현해야 함
	// TODO : 재장전 조건 추가 필요 (현재 탄창이 최대 탄창과 같으면 x)
	// 재장전
	if (KEY_TAP(CPlayerController::Reload))
		return (int)PLAYER_STATE::StandReload;

	// 사격 준비
	if (KEY_TAP(CPlayerController::Zoom) || KEY_PRESSED(CPlayerController::Zoom))
		return (int)PLAYER_STATE::StandAttackStart;

	if (KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack))
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
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::StandAttackDelay;
	if (KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack))
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
}

int CPlayerScript::StandAttackIngUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::StandAttackDelay;

	return m_FSM->GetCurState();
}

void CPlayerScript::StandAttackIngEnd()
{
}

void CPlayerScript::StandAttackDelayBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::StandAttackDelay);
}

int CPlayerScript::StandAttackDelayUpdate()
{
	if (KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack))
		return (int)PLAYER_STATE::StandAttackIng;

	if (KEY_TAP(CPlayerController::Reload))
		return (int)PLAYER_STATE::StandReload;

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
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::StandIdle;
	if (KEY_TAP(CPlayerController::Zoom))
		return (int)PLAYER_STATE::StandAttackStart;

	if (KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack))
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
	float length = Animator3D()->GetAnimLength((int)PLAYER_STATE::KneelIdle);
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

	if (KEY_TAP(CPlayerController::Jump))
		return (int)PLAYER_STATE::MoveJump;

	if (KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack))
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
	float length = Animator3D()->GetAnimLength((int)PLAYER_STATE::KneelAttackStart);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::KneelAttackStart], 0.1f);
}

int CPlayerScript::KneelAttackStartUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::KneelAttackDelay;

	if (KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack))
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
}

int CPlayerScript::KneelAttackIngUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::KneelAttackDelay;

	if (KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack))
		return (int)PLAYER_STATE::KneelAttackIng;
	return m_FSM->GetCurState();
}

void CPlayerScript::KneelAttackIngEnd()
{
}

void CPlayerScript::KneelAttackDelayBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::KneelAttackDelay);
}

int CPlayerScript::KneelAttackDelayUpdate()
{
	if (KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack))
		return (int)PLAYER_STATE::KneelAttackIng;

	if (KEY_TAP(CPlayerController::Reload))
		return (int)PLAYER_STATE::KneelReload;

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
	float length = Animator3D()->GetAnimLength((int)PLAYER_STATE::KneelAttackEnd);
	m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::KneelIdle], length);
}

int CPlayerScript::KneelAttackEndUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::KneelIdle;
	if (KEY_TAP(CPlayerController::Zoom))
		return (int)PLAYER_STATE::KneelAttackStart;

	if (KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack))
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
	if (MoveEndCondition)
		return (int)PLAYER_STATE::MoveEndNormal;

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
	if (MoveEndCondition)
		return (int)PLAYER_STATE::MoveEndStand;

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
	if (MoveEndCondition)
		return (int)PLAYER_STATE::MoveEndKneel;

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
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::NormalIdle;

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
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::StandIdle;
	// 사격 준비
	if (KEY_TAP(CPlayerController::Zoom) || KEY_PRESSED(CPlayerController::Zoom))
		return (int)PLAYER_STATE::StandAttackStart;

	if (KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack))
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
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::KneelIdle;
	// 사격 준비
	if (KEY_TAP(CPlayerController::Zoom) || KEY_PRESSED(CPlayerController::Zoom))
		return (int)PLAYER_STATE::KneelAttackStart;
	if (KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack))
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

	if (KEY_TAP(CPlayerController::Zoom))
		return (int)PLAYER_STATE::NormalAttackStart;

	// TODO: Reload 조건
	if (KEY_TAP(CPlayerController::Reload))
		return (int)PLAYER_STATE::NormalReload;

	// 스킬
	if (KEY_TAP(CPlayerController::Dash))
		return (int)PLAYER_STATE::SkillDash;

	return m_FSM->GetCurState();
}

void CPlayerScript::MoveIngEnd()
{
}

void CPlayerScript::MoveJumpBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveJump, 0);
}

int CPlayerScript::MoveJumpUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::NormalIdle;

	return m_FSM->GetCurState();
}

void CPlayerScript::MoveJumpEnd()
{
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
}

int CPlayerScript::VictoryStartUpdate()
{
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
}

int CPlayerScript::SkillDashUpdate()
{
	if (!Animator3D()->IsPlayable())
	{
		return SwitchToCoverTypeIdle();
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::SkillDashEnd()
{
}

void CPlayerScript::SkillThrowBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::SkillThrow, 0);
}

int CPlayerScript::SkillThrowUpdate()
{
	if (!Animator3D()->IsPlayable())
	{
		return SwitchToCoverTypeIdle();
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::SkillThrowEnd()
{
}

void CPlayerScript::SkillCallsignBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::SkillCallsign, 0);
}

int CPlayerScript::SkillCallsignUpdate()
{
	if (!Animator3D()->IsPlayable())
	{
		return SwitchToCoverTypeIdle();
	}

	return m_FSM->GetCurState();
}

void CPlayerScript::SkillCallsignEnd()
{
}

void CPlayerScript::SkillEXBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::SkillEX, 0);
}

int CPlayerScript::SkillEXUpdate()
{
	if (!Animator3D()->IsPlayable())
	{
		return SwitchToCoverTypeIdle();
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
	Animator3D()->Play((int)PLAYER_STATE::FormationIdle);
}

int CPlayerScript::FormationIdleUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return 0;
	}
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
