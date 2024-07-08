﻿#include "pch.h"
#include "CPlayerScript.h"

#include "CRoRStateMachine.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CKeyMgr.h>

#include "CPlayerController.h"

#define MoveStartCondition                                                              \
	(KEY_TAP(CPlayerController::Front) || KEY_PRESSED(CPlayerController::Front)) ||     \
		(KEY_TAP(CPlayerController::Back) || KEY_PRESSED(CPlayerController::Back)) ||   \
		(KEY_TAP(CPlayerController::Right) || KEY_PRESSED(CPlayerController::Right)) || \
		(KEY_TAP(CPlayerController::Left) || KEY_PRESSED(CPlayerController::Left))

#define MoveEndCondition                                                                  \
	(KEY_RELEASED(CPlayerController::Front) || KEY_NONE(CPlayerController::Front)) &&     \
		(KEY_RELEASED(CPlayerController::Back) || KEY_NONE(CPlayerController::Back)) &&   \
		(KEY_RELEASED(CPlayerController::Right) || KEY_NONE(CPlayerController::Right)) && \
		(KEY_RELEASED(CPlayerController::Left) || KEY_NONE(CPlayerController::Left))

#define MOVEEND                                  \
	if (MoveEndCondition)                        \
	{                                            \
		return (int)PLAYER_STATE::MoveEndNormal; \
	}

#pragma region Normal

void CPlayerScript::NormalIdleBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::NormalIdle);
}

int CPlayerScript::NormalIdleUpdate()
{
	// TODO: 상하 에임 구현해야 함
	// TODO : 재장전 조건 추가 필요 (현재 탄창이 최대 탄창과 같으면 x)
	// 재장전
	if (KEY_TAP(CPlayerController::Reload))
	{
		return (int)PLAYER_STATE::NormalReload;
	}

	// 사격 준비
	if (KEY_TAP(CPlayerController::Zoom))
	{
		return (int)PLAYER_STATE::NormalAttackStart;
	}

	// 이동
	if (MoveStartCondition)
	{
		return (int)PLAYER_STATE::MoveStartNormal;
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
	{
		return (int)PLAYER_STATE::NormalIdle;
	}

	return m_FSM->GetCurState();
}

void CPlayerScript::NormalReloadEnd()
{
}

void CPlayerScript::NormalAttackStartBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::NormalAttackStart, 0);
}

int CPlayerScript::NormalAttackStartUpdate()
{
	if (!Animator3D()->IsPlayable())
	{
		return (int)PLAYER_STATE::NormalAttackDelay;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::NormalAttackStartEnd()
{
}

void CPlayerScript::NormalAttackIngBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::NormalAttackIng, 0);
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
	if (KEY_TAP(CPlayerController::Attack))
	{
		return (int)PLAYER_STATE::NormalAttackIng;
	}

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
}

int CPlayerScript::NormalAttackEndUpdate()
{
	if (!Animator3D()->IsPlayable())
	{
		return (int)PLAYER_STATE::NormalIdle;
	}
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
}

int CPlayerScript::StandIdleUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::StandIdleEnd()
{
}

void CPlayerScript::StandReloadBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::StandReload);
}

int CPlayerScript::StandReloadUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::StandReloadEnd()
{
}

void CPlayerScript::StandAttackStartBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::StandAttackStart);
}

int CPlayerScript::StandAttackStartUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::StandAttackStartEnd()
{
}

void CPlayerScript::StandAttackIngBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::StandAttackIng);
}

int CPlayerScript::StandAttackIngUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
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
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::StandAttackDelayEnd()
{
}

void CPlayerScript::StandAttackEndBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::StandAttackEnd);
}

int CPlayerScript::StandAttackEndUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
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
}

int CPlayerScript::KneelIdleUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::KneelIdleEnd()
{
}

void CPlayerScript::KneelReloadBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::KneelReload);
}

int CPlayerScript::KneelReloadUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::KneelReloadEnd()
{
}

void CPlayerScript::KneelAttackStartBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::KneelAttackStart);
}

int CPlayerScript::KneelAttackStartUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::KneelAttackStartEnd()
{
}

void CPlayerScript::KneelAttackIngBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::KneelAttackIng);
}

int CPlayerScript::KneelAttackIngUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
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
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::KneelAttackDelayEnd()
{
}

void CPlayerScript::KneelAttackEndBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::KneelAttackEnd);
}

int CPlayerScript::KneelAttackEndUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
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
}

int CPlayerScript::MoveStartNormalUpdate()
{
	MOVEEND;

	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::MoveIng;
	return m_FSM->GetCurState();
}

void CPlayerScript::MoveStartNormalEnd()
{
}

void CPlayerScript::MoveStartStandBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveStartStand);
}

int CPlayerScript::MoveStartStandUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::MoveStartStandEnd()
{
}

void CPlayerScript::MoveStartKneelBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveStartKneel);
}

int CPlayerScript::MoveStartKneelUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::MoveStartKneelEnd()
{
}

void CPlayerScript::MoveEndNormalBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveEndNormal, 0);
}

int CPlayerScript::MoveEndNormalUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::NormalIdle;
	return m_FSM->GetCurState();
}

void CPlayerScript::MoveEndNormalEnd()
{
}

void CPlayerScript::MoveEndStandBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveEndStand, 0);
}

int CPlayerScript::MoveEndStandUpdate()
{
	if (!Animator3D()->IsPlayable())
		return (int)PLAYER_STATE::StandIdle;
	return m_FSM->GetCurState();
}

void CPlayerScript::MoveEndStandEnd()
{
}

void CPlayerScript::MoveEndKneelBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveEndKneel);
}

int CPlayerScript::MoveEndKneelUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::MoveEndKneelEnd()
{
}

void CPlayerScript::MoveIngBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveIng);
}

int CPlayerScript::MoveIngUpdate()
{

	// TODO: 엄폐 조건에 따라 노말 보낼지 stand 보낼지, kneel 보낼지 결정 줘야함
	MOVEEND;

	return m_FSM->GetCurState();
}

void CPlayerScript::MoveIngEnd()
{
}

void CPlayerScript::MoveJumpBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveJump);
}

int CPlayerScript::MoveJumpUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::MoveJumpEnd()
{
}

#pragma endregion

#pragma region Vital

void CPlayerScript::VitalDeathBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::VitalDeath);
}

int CPlayerScript::VitalDeathUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::VitalDeathEnd()
{
}

void CPlayerScript::VitalPanicBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::VitalPanic);
}

int CPlayerScript::VitalPanicUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::VitalPanicEnd()
{
}

void CPlayerScript::VitalDyingBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::VitalDying);
}

int CPlayerScript::VitalDyingUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::VitalDyingEnd()
{
}

#pragma endregion

#pragma region Victory

void CPlayerScript::VictoryStartBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::VictoryStart);
}

int CPlayerScript::VictoryStartUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::VictoryStartEnd()
{
}

void CPlayerScript::VictoryEndBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::VictoryEnd);
}

int CPlayerScript::VictoryEndUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::VictoryEndEnd()
{
}

#pragma endregion

#pragma region Skill

void CPlayerScript::SkillDashBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::SkillDash);
}

int CPlayerScript::SkillDashUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::SkillDashEnd()
{
}

void CPlayerScript::SkillThrowBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::SkillThrow);
}

int CPlayerScript::SkillThrowUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::SkillThrowEnd()
{
}

void CPlayerScript::SkillCallsignBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::SkillCallsign);
}

int CPlayerScript::SkillCallsignUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::SkillCallsignEnd()
{
}

void CPlayerScript::SkillEXBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::SkillEX);
}

int CPlayerScript::SkillEXUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
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
