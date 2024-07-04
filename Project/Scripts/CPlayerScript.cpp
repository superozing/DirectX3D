#include "pch.h"
#include "CPlayerScript.h"
#include <Engine/CAssetMgr.h>

#include <Engine/CGameObject.h>
#include "CMissileScript.h"

#include <Engine/CMaterial.h>
#include <Engine/CRenderComponent.h>

#include <Engine\CLogMgr.h>
#include "CRoRStateMachine.h"

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_tStatus{}
{
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

	// 스테이트 초기화
	m_FSM = new CRoRStateMachine<CPlayerScript>(this, (UINT)PLAYER_STATE::END);

	m_FSM->SetCallbacks((UINT)PLAYER_STATE::NORMAL, ToString(magic_enum::enum_name(PLAYER_STATE::NORMAL)),
						&CPlayerScript::NormalUpdate, &CPlayerScript::NormalBegin, &CPlayerScript::NormalEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::ATTACK, ToString(magic_enum::enum_name(PLAYER_STATE::ATTACK)),
						&CPlayerScript::AttackUpdate, &CPlayerScript::AttackBegin, &CPlayerScript::AttackEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::COVER, ToString(magic_enum::enum_name(PLAYER_STATE::COVER)),
						&CPlayerScript::CoverUpdate, &CPlayerScript::CoverBegin, &CPlayerScript::CoverEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::COVERATTACK, ToString(magic_enum::enum_name(PLAYER_STATE::COVERATTACK)),
						&CPlayerScript::CoverAttackUpdate, &CPlayerScript::CoverAttackBegin,
						&CPlayerScript::CoverAttackEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::COVERKNEEATTACK,
						ToString(magic_enum::enum_name(PLAYER_STATE::COVERKNEEATTACK)),
						&CPlayerScript::CoverKneeAttackUpdate, &CPlayerScript::CoverKneeAttackBegin,
						&CPlayerScript::CoverKneeAttackEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::DEATH, ToString(magic_enum::enum_name(PLAYER_STATE::DEATH)),
						&CPlayerScript::DeathUpdate, &CPlayerScript::DeathBegin, &CPlayerScript::DeathEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::CEREMONY, ToString(magic_enum::enum_name(PLAYER_STATE::CEREMONY)),
						&CPlayerScript::CeremonyUpdate, &CPlayerScript::CeremonyBegin, &CPlayerScript::CeremonyEnd,
						nullptr);
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

static string state = "";
#include "CSpringArm.h"
#include <Engine/CRenderMgr.h>

void CPlayerScript::begin()
{
	m_FSM->Begin();
	AppendScriptParam("CurState", SCRIPT_PARAM::STRING, (void*)&state);

	auto pSA = GetOwner()->GetScript<CSpringArm>();
	if (pSA)
	{
		pSA->SetTargetCamera(CRenderMgr::GetInst()->GetMainCam());
	}
}

void CPlayerScript::tick()
{
	m_FSM->Update();
	state = magic_enum::enum_name((PLAYER_STATE)m_FSM->GetCurState());
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
