#include "pch.h"
#include "CPlayerScript.h"

#include "CRoRStateMachine.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CKeyMgr.h>

#include "CPlayerController.h"

#pragma region Normal

void CPlayerScript::NormalIdleBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::NormalIdle);
}

int CPlayerScript::NormalIdleUpdate()
{
	// TODO: 마우스 x축 회전에 따라 스프링암의 y축이 회전해야 함
	// TODO: 키 입력 커스터마이징 가능하도록 민서 Controller 완성되면 붙여야 함

	// 카메라에 따라 캐릭터 8방향 움직임 - 스프링 암에서 가져와야 함
	// 마지막 키 입력 추적해야 함 - 앞, 뒤랑 양 옆을
	CGameObject* pCamera = CRenderMgr::GetInst()->GetMainCam()->GetOwner();

	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();

	Vec2  vMouseDiff = CKeyMgr::GetInst()->GetMouseDrag();
	float CamSpeed	 = 3.f;
	if (vMouseDiff.x > 0.f)
	{
		vRot.y += CamSpeed * DT;
	}
	else if (vMouseDiff.x < 0.f)
	{
		vRot.y -= CamSpeed * DT;
	}

	if (KEY_PRESSED(CPlayerController::Front))
	{
		vPos += vFront * m_tStatus.MoveSpeed * DT;
	}
	if (KEY_PRESSED(CPlayerController::Back))
	{
		vPos -= vFront * m_tStatus.MoveSpeed * DT;
	}
	if (KEY_PRESSED(CPlayerController::Right))
	{
		vPos += vRight * m_tStatus.MoveSpeed * DT;
	}
	if (KEY_PRESSED(CPlayerController::Left))
	{
		vPos -= vRight * m_tStatus.MoveSpeed * DT;
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRot);

	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::NormalIdleEnd()
{
}

void CPlayerScript::NormalReloadBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::NormalReload);
}

int CPlayerScript::NormalReloadUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::NormalReloadEnd()
{
}

void CPlayerScript::NormalAttackStartBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::NormalAttackStart);
}

int CPlayerScript::NormalAttackStartUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::NormalAttackStartEnd()
{
}

void CPlayerScript::NormalAttackIngBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::NormalAttackIng);
}

int CPlayerScript::NormalAttackIngUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
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
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::NormalAttackDelayEnd()
{
}

void CPlayerScript::NormalAttackEndBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::NormalAttackEnd);
}

int CPlayerScript::NormalAttackEndUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
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
	Animator3D()->Play((int)PLAYER_STATE::MoveStartNormal);
}

int CPlayerScript::MoveStartNormalUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
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
	Animator3D()->Play((int)PLAYER_STATE::MoveEndNormal);
}

int CPlayerScript::MoveEndNormalUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::MoveEndNormalEnd()
{
}

void CPlayerScript::MoveEndStandBegin()
{
	Animator3D()->Play((int)PLAYER_STATE::MoveEndStand);
}

int CPlayerScript::MoveEndStandUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
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
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
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
