#include "pch.h"
#include "CPlayerScript.h"

#include "CRoRStateMachine.h"
#include <Engine/CRenderMgr.h>

void CPlayerScript::NormalBegin()
{
	// 노말 애니메이션 시작
	Animator3D()->Play(2);
}

int CPlayerScript::NormalUpdate()
{
	// TODO: 마우스 x축 회전에 따라 스프링암의 y축이 회전해야 함
	// TODO: 키 입력 커스터마이징 가능하도록 민서 Controller 완성되면 붙여야 함

	// 카메라에 따라 캐릭터 8방향 움직임 - 스프링 암에서 가져와야 함
	// 마지막 키 입력 추적해야 함 - 앞, 뒤랑 양 옆을
	CGameObject* pCamera   = CRenderMgr::GetInst()->GetMainCam()->GetOwner();
	Vec3		 vCamFront = pCamera->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3		 vCamRight = pCamera->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3		 vTargetDir;
	if (KEY_PRESSED(W))
	{
		vTargetDir += vCamFront;
	}
	if (KEY_PRESSED(S))
	{
		vTargetDir += -vCamFront;
	}
	if (KEY_PRESSED(D))
	{
		vTargetDir += vCamRight;
	}
	if (KEY_PRESSED(A))
	{
		vTargetDir += -vCamRight;
	}

	Vec3 vPos	= Transform()->GetRelativePos();
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	if (vTargetDir != Vec3())
	{
		vPos += vFront * m_Speed * DT;
		float fAngleSpeed = 20.f;
		vTargetDir		  = Vec3::Lerp(Transform()->GetWorldDir(DIR_TYPE::FRONT), vTargetDir, DT * fAngleSpeed);
		Transform()->SetDir(vTargetDir);
	}

	Transform()->SetRelativePos(vPos);

	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::NormalEnd()
{
}

static float	   Att_Acctime	= 0.f;
static const float Att_duration = 1.f;

void CPlayerScript::AttackBegin()
{
	Att_Acctime = 0.f;
}

int CPlayerScript::AttackUpdate()
{
	// 기본 움직임
	Vec3 vPos	= Transform()->GetRelativePos();
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	if (KEY_PRESSED(W))
	{
		vPos += vFront * m_Speed * DT;
	}
	if (KEY_PRESSED(S))
	{
		vPos += -vFront * m_Speed * DT;
	}
	if (KEY_PRESSED(D))
	{
		vPos += vRight * m_Speed * DT;
	}
	if (KEY_PRESSED(A))
	{
		vPos += -vRight * m_Speed * DT;
	}

	Transform()->SetRelativePos(vPos);

	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::AttackEnd()
{
}

void CPlayerScript::CoverBegin()
{
}

int CPlayerScript::CoverUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::CoverEnd()
{
}

void CPlayerScript::CoverAttackBegin()
{
}

int CPlayerScript::CoverAttackUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::CoverAttackEnd()
{
}

void CPlayerScript::CoverKneeAttackBegin()
{
}

int CPlayerScript::CoverKneeAttackUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::CoverKneeAttackEnd()
{
}

void CPlayerScript::DeathBegin()
{
}

int CPlayerScript::DeathUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::DeathEnd()
{
}

void CPlayerScript::CeremonyBegin()
{
}

int CPlayerScript::CeremonyUpdate()
{
	if (KEY_TAP(KEY::SPACE))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CPlayerScript::CeremonyEnd()
{
}
