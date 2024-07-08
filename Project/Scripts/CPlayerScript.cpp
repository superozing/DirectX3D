﻿#include "pch.h"
#include "CPlayerScript.h"
#include <Engine/CAssetMgr.h>

#include <Engine/CGameObject.h>
#include "CMissileScript.h"

#include <Engine/CMaterial.h>
#include <Engine/CRenderComponent.h>

#include <Engine/CKeyMgr.h>

#include "CRoRStateMachine.h"
#include "CPlayerController.h"
#include "CSpringArm.h"

static string state = "";
static string cover = "";

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_tStatus{}
	, m_pSpringArm(nullptr)
{
	// 테스트용
	AppendScriptParam("CurState", SCRIPT_PARAM::STRING, (void*)&state);
	AppendScriptParam("CoverType", SCRIPT_PARAM::STRING, (void*)&cover);

	AppendScriptParam("IsDead", SCRIPT_PARAM::BOOL, &m_tStatus.IsDead, 0, 0, true);
	AppendScriptParam("Damage", SCRIPT_PARAM::FLOAT, &m_tStatus.Damage);
	AppendScriptParam("Health", SCRIPT_PARAM::FLOAT, &m_tStatus.curHealth);
	AppendScriptParam("Stamina", SCRIPT_PARAM::FLOAT, &m_tStatus.curStamina);
	AppendScriptParam("Defensive", SCRIPT_PARAM::FLOAT, &m_tStatus.Defensive);
	AppendScriptParam("Avoid Rate", SCRIPT_PARAM::FLOAT, &m_tStatus.AvoidPercent);
	AppendScriptParam("Critical Rate", SCRIPT_PARAM::FLOAT, &m_tStatus.CriticalPercent);
	AppendScriptParam("Critical Damage", SCRIPT_PARAM::FLOAT, &m_tStatus.CriticalDamage);
	AppendScriptParam("MoveSpeed", SCRIPT_PARAM::FLOAT, &m_tStatus.MoveSpeed);
	AppendScriptParam("AttackMoveSpeed", SCRIPT_PARAM::FLOAT, &m_tStatus.AttackMoveSpeed);

#pragma region StateMachineInit

	// 스테이트 초기화
	m_FSM = new CRoRStateMachine<CPlayerScript>(this, (UINT)PLAYER_STATE::END);

	FSMInit(PLAYER_STATE, CPlayerScript, NormalIdle);
	FSMInit(PLAYER_STATE, CPlayerScript, NormalReload);
	FSMInit(PLAYER_STATE, CPlayerScript, NormalAttackStart);
	FSMInit(PLAYER_STATE, CPlayerScript, NormalAttackIng);
	FSMInit(PLAYER_STATE, CPlayerScript, NormalAttackDelay);
	FSMInit(PLAYER_STATE, CPlayerScript, NormalAttackEnd);

	FSMInit(PLAYER_STATE, CPlayerScript, StandIdle);
	FSMInit(PLAYER_STATE, CPlayerScript, StandReload);
	FSMInit(PLAYER_STATE, CPlayerScript, StandAttackStart);
	FSMInit(PLAYER_STATE, CPlayerScript, StandAttackIng);
	FSMInit(PLAYER_STATE, CPlayerScript, StandAttackDelay);
	FSMInit(PLAYER_STATE, CPlayerScript, StandAttackEnd);

	FSMInit(PLAYER_STATE, CPlayerScript, KneelIdle);
	FSMInit(PLAYER_STATE, CPlayerScript, KneelReload);
	FSMInit(PLAYER_STATE, CPlayerScript, KneelAttackStart);
	FSMInit(PLAYER_STATE, CPlayerScript, KneelAttackIng);
	FSMInit(PLAYER_STATE, CPlayerScript, KneelAttackDelay);
	FSMInit(PLAYER_STATE, CPlayerScript, KneelAttackEnd);

	FSMInit(PLAYER_STATE, CPlayerScript, MoveStartNormal);
	FSMInit(PLAYER_STATE, CPlayerScript, MoveStartStand);
	FSMInit(PLAYER_STATE, CPlayerScript, MoveStartKneel);
	FSMInit(PLAYER_STATE, CPlayerScript, MoveEndNormal);
	FSMInit(PLAYER_STATE, CPlayerScript, MoveEndStand);
	FSMInit(PLAYER_STATE, CPlayerScript, MoveEndKneel);
	FSMInit(PLAYER_STATE, CPlayerScript, MoveIng);
	FSMInit(PLAYER_STATE, CPlayerScript, MoveJump);

	FSMInit(PLAYER_STATE, CPlayerScript, VitalDeath);
	FSMInit(PLAYER_STATE, CPlayerScript, VitalPanic);
	FSMInit(PLAYER_STATE, CPlayerScript, VitalDying);
	FSMInit(PLAYER_STATE, CPlayerScript, VictoryStart);
	FSMInit(PLAYER_STATE, CPlayerScript, VictoryEnd);

	FSMInit(PLAYER_STATE, CPlayerScript, SkillDash);
	FSMInit(PLAYER_STATE, CPlayerScript, SkillThrow);
	FSMInit(PLAYER_STATE, CPlayerScript, SkillCallsign);
	FSMInit(PLAYER_STATE, CPlayerScript, SkillEX);

	FSMInit(PLAYER_STATE, CPlayerScript, FormationIdle);

#pragma endregion
}

CPlayerScript::CPlayerScript(const CPlayerScript& _origin)
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_tStatus(_origin.m_tStatus)
{
	m_FSM = _origin.m_FSM->Clone(this);
}

CPlayerScript::~CPlayerScript()
{
	if (nullptr != m_FSM)
	{
		delete m_FSM;
		m_FSM = nullptr;
	}
}

#include <Engine/CRenderMgr.h>

void CPlayerScript::begin()
{
	m_FSM->Begin();

	auto vecChild = GetOwner()->GetChild();
	for (size_t i = 0; i < vecChild.size(); i++)
	{
		m_pSpringArm = vecChild[i]->GetScript<CSpringArm>();
		if (m_pSpringArm)
			break;
	}

	if (m_pSpringArm)
		m_pSpringArm->SetTargetObject(CRenderMgr::GetInst()->GetMainCam()->GetOwner());
}

void CPlayerScript::tick()
{
	m_FSM->Update();
	state = magic_enum::enum_name((PLAYER_STATE)m_FSM->GetCurState());
	cover = magic_enum::enum_name((CoverType)GetCoverType());

	CameraRotation();
	NormalMove();

	// 엄폐 판정 할 수 있게되면 지울 함수
	SwitchCoverType();
}

void CPlayerScript::CameraRotation()
{
	auto state = m_FSM->GetCurState();
	if (state != (int)PLAYER_STATE::NormalIdle && state != (int)PLAYER_STATE::NormalReload &&
		state != (int)PLAYER_STATE::NormalAttackStart && state != (int)PLAYER_STATE::NormalAttackDelay &&
		state != (int)PLAYER_STATE::NormalAttackIng && state != (int)PLAYER_STATE::NormalAttackEnd &&
		state != (int)PLAYER_STATE::MoveStartNormal && state != (int)PLAYER_STATE::MoveEndNormal &&
		state != (int)PLAYER_STATE::MoveIng)
		return;

	Vec3 vRot		= Transform()->GetRelativeRotation();
	Vec2 vMouseDiff = CKeyMgr::GetInst()->GetMouseDrag();
	if (vMouseDiff.x > 0.f)
		vRot.y += CPlayerController::Sensitivity * DT;
	else if (vMouseDiff.x < 0.f)
		vRot.y -= CPlayerController::Sensitivity * DT;

	if (m_pSpringArm && m_pSpringArm->IsActivate())
	{
		float fYSpeed = 100.f;
		Vec3  vOffset = m_pSpringArm->GetDirOffset();
		if (vMouseDiff.y > 0.f)
			vOffset.y -= CPlayerController::Sensitivity * fYSpeed * DT;
		else if (vMouseDiff.y < 0.f)
			vOffset.y += CPlayerController::Sensitivity * fYSpeed * DT;

		m_pSpringArm->SetDirOffset(vOffset);
	}

	Transform()->SetRelativeRotation(vRot);
}

void CPlayerScript::NormalMove()
{
	auto state = m_FSM->GetCurState();
	if (state != (int)PLAYER_STATE::MoveStartNormal && state != (int)PLAYER_STATE::MoveEndNormal &&
		state != (int)PLAYER_STATE::MoveIng)
		return;

	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vPos	= Transform()->GetRelativePos();

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
}

int CPlayerScript::SwitchToCoverTypeIdle()
{
	switch (GetCoverType())
	{
	case CoverType::Normal:
		return (int)PLAYER_STATE::NormalIdle;
		break;
	case CoverType::Stand:
		return (int)PLAYER_STATE::StandIdle;
		break;
	case CoverType::Kneel:
		return (int)PLAYER_STATE::KneelIdle;
		break;
	}

	return (int)PLAYER_STATE::END;
}

void CPlayerScript::SwitchCoverType()
{
	if (KEY_TAP(KEY::_1))
		SetCoverType(CoverType::Normal);
	if (KEY_TAP(KEY::_2))
		SetCoverType(CoverType::Stand);
	if (KEY_TAP(KEY::_3))
		SetCoverType(CoverType::Kneel);
}

void CPlayerScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	Ptr<CMaterial> pMtrl = GetRenderComponent()->GetDynamicMaterial(0);
}

void CPlayerScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::SaveToFile(FILE* _File)
{
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
}
