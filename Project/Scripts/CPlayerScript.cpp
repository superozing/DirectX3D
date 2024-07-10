#include "pch.h"
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

#pragma region CameraSetting

	SpringArmInfo info;
	info.Type								 = true;
	info.fMaxDistance						 = 150.f;
	info.fCamSpeed							 = 50.f;
	info.fCamRotSpeed						 = 20.f;
	info.vDir								 = Vec3(-20.f, 180.f, 0.f);
	info.vOffsetPos							 = Vec2(50.f, 50.f);
	m_mSpringInfos[PLAYER_STATE::NormalIdle] = info;

	info.Type										= false;
	info.fMaxDistance								= 70.f;
	info.fCamSpeed									= 30.f;
	info.fCamRotSpeed								= 20.f;
	info.vDir										= Vec3(-20.f, 180.f, 0.f);
	info.vOffsetPos									= Vec2(50.f, 50.f);
	m_mSpringInfos[PLAYER_STATE::NormalAttackStart] = info;

	// info.Type									  = false;
	// info.fMaxDistance							  = 250.f;
	// info.fCamSpeed								  = 30.f;
	// info.fCamRotSpeed							  = 20.f;
	// info.vDir									  = Vec3(0.f, 0.4f, -1.f);
	// info.vOffsetPos								  = Vec2(150.f, 250.f);
	// m_mSpringInfos[PLAYER_STATE::NormalAttackEnd] = info;

	info.Type								= false;
	info.fMaxDistance						= 150.f;
	info.fCamSpeed							= 30.f;
	info.fCamRotSpeed						= 20.f;
	info.vDir								= Vec3(-20.f, 180.f, 0.f);
	info.vOffsetPos							= Vec2(100.f, 50.f);
	m_mSpringInfos[PLAYER_STATE::StandIdle] = info;

	info.Type									   = false;
	info.fMaxDistance							   = 70.f;
	info.fCamSpeed								   = 30.f;
	info.fCamRotSpeed							   = 20.f;
	info.vDir									   = Vec3(-20.f, 180.f, 0.f);
	info.vOffsetPos								   = Vec2(100.f, 50.f);
	m_mSpringInfos[PLAYER_STATE::StandAttackStart] = info;

	info.Type								= true;
	info.fMaxDistance						= 150.f;
	info.fCamSpeed							= 30.f;
	info.fCamRotSpeed						= 20.f;
	info.vDir								= Vec3(-20.f, 180.f, 0.f);
	info.vOffsetPos							= Vec2(200.f, 150.f);
	m_mSpringInfos[PLAYER_STATE::KneelIdle] = info;

	info.Type									   = false;
	info.fMaxDistance							   = 150.f;
	info.fCamSpeed								   = 30.f;
	info.fCamRotSpeed							   = 20.f;
	info.vDir									   = Vec3(-20.f, 180.f, 0.f);
	info.vOffsetPos								   = Vec2(300.f, 200.f);
	m_mSpringInfos[PLAYER_STATE::KneelAttackStart] = info;

#pragma endregion
}

CPlayerScript::CPlayerScript(const CPlayerScript& _origin)
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_tStatus(_origin.m_tStatus)
	, m_mSpringInfos(_origin.m_mSpringInfos)
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
	auto vecChild = GetOwner()->GetChild();
	for (size_t i = 0; i < vecChild.size(); i++)
	{
		m_pSpringArm = vecChild[i]->GetScript<CSpringArm>();
		if (m_pSpringArm)
			break;
	}

	if (m_pSpringArm)
		m_pSpringArm->SetTargetObject(CRenderMgr::GetInst()->GetMainCam()->GetOwner());

	m_FSM->Begin();
}

void CPlayerScript::tick()
{
	m_FSM->Update();
	state = magic_enum::enum_name((PLAYER_STATE)m_FSM->GetCurState());
	cover = magic_enum::enum_name((CoverType)GetCoverType());

	CameraMove();
	NormalMove();

	// 엄폐 판정 할 수 있게되면 지울 함수
	SwitchCoverType();
}

void CPlayerScript::CameraMove()
{
	auto state = m_FSM->GetCurState();

	Vec3 vRot		= Transform()->GetRelativeRotation();
	Vec2 vMouseDiff = CKeyMgr::GetInst()->GetMouseDrag();

	// 캐릭터 좌우 회전
	if (state == (int)PLAYER_STATE::NormalIdle || state == (int)PLAYER_STATE::NormalReload ||
		state == (int)PLAYER_STATE::NormalAttackStart || state == (int)PLAYER_STATE::NormalAttackDelay ||
		state == (int)PLAYER_STATE::NormalAttackIng || state == (int)PLAYER_STATE::NormalAttackEnd ||
		state == (int)PLAYER_STATE::MoveStartNormal || state == (int)PLAYER_STATE::MoveEndNormal ||
		state == (int)PLAYER_STATE::MoveIng)
	{

		if (vMouseDiff.x > 0.f)
			vRot.y += CPlayerController::Sensitivity * DT;
		else if (vMouseDiff.x < 0.f)
			vRot.y -= CPlayerController::Sensitivity * DT;
		Transform()->SetRelativeRotation(vRot);
	}

	// 카메라 상하 회전
	if (state == (int)PLAYER_STATE::NormalIdle || state == (int)PLAYER_STATE::NormalReload ||
		state == (int)PLAYER_STATE::NormalAttackStart || state == (int)PLAYER_STATE::NormalAttackDelay ||
		state == (int)PLAYER_STATE::NormalAttackIng || state == (int)PLAYER_STATE::NormalAttackEnd ||
		state == (int)PLAYER_STATE::StandIdle || state == (int)PLAYER_STATE::StandReload ||
		state == (int)PLAYER_STATE::StandAttackStart || state == (int)PLAYER_STATE::StandAttackDelay ||
		state == (int)PLAYER_STATE::StandAttackIng || state == (int)PLAYER_STATE::StandAttackEnd ||
		state == (int)PLAYER_STATE::KneelIdle || state == (int)PLAYER_STATE::KneelReload ||
		state == (int)PLAYER_STATE::KneelAttackStart || state == (int)PLAYER_STATE::KneelAttackDelay ||
		state == (int)PLAYER_STATE::KneelAttackIng || state == (int)PLAYER_STATE::KneelAttackEnd ||
		state == (int)PLAYER_STATE::MoveStartNormal || state == (int)PLAYER_STATE::MoveEndNormal ||
		state == (int)PLAYER_STATE::MoveIng)
	{
		if (m_pSpringArm && m_pSpringArm->IsActivate())
		{
			float CamRotSpeed = 10.f;
			Vec3  vOffset	  = m_pSpringArm->GetDirOffset();
			if (vMouseDiff.y > 0.f)
				vOffset.y += CPlayerController::Sensitivity * CamRotSpeed * DT;
			else if (vMouseDiff.y < 0.f)
				vOffset.y -= CPlayerController::Sensitivity * CamRotSpeed * DT;

			// 카메라 좌우 회전
			if (state == (int)PLAYER_STATE::StandIdle || state == (int)PLAYER_STATE::StandReload ||
				state == (int)PLAYER_STATE::StandAttackStart || state == (int)PLAYER_STATE::StandAttackDelay ||
				state == (int)PLAYER_STATE::StandAttackIng || state == (int)PLAYER_STATE::StandAttackEnd ||
				state == (int)PLAYER_STATE::KneelIdle || state == (int)PLAYER_STATE::KneelReload ||
				state == (int)PLAYER_STATE::KneelAttackStart || state == (int)PLAYER_STATE::KneelAttackDelay ||
				state == (int)PLAYER_STATE::KneelAttackIng || state == (int)PLAYER_STATE::KneelAttackEnd)
			{
				if (vMouseDiff.x > 0.f)
					vOffset.x += CPlayerController::Sensitivity * CamRotSpeed * DT;
				else if (vMouseDiff.x < 0.f)
					vOffset.x -= CPlayerController::Sensitivity * CamRotSpeed * DT;
			}
			else
			{
				vOffset.x = 0.f;
			}

			m_pSpringArm->SetDirOffset(vOffset);
		}
	}
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
