#include "pch.h"
#include "CBossScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CRandomMgr.h>
#include <Engine\CAssetMgr.h>
#include "CRoRStateMachine.h"
#include "CMegaFistScript.h"
#include "CBossMissileScript.h"
#include "CBossShieldScript.h"

static string DebugState = "";

CBossScript::CBossScript()
	: CScript((UINT)SCRIPT_TYPE::BOSSSCRIPT)
	, m_BossStatus{}
	, m_AttDuration(0.f)
	, m_EXsDuration(0.f)
	, m_ActiveAttack(false)
	, m_ActiveEXs(false)
	, m_EXsType(0)
	, m_Target(nullptr)
	, m_ArrMissile{}
	, m_ActiveShield(false)
{
	AppendScriptParam("CurState    ", SCRIPT_PARAM::STRING, &DebugState);

	InitScriptParamUI();

	InitStateMachine();
}

CBossScript::CBossScript(const CBossScript& _Origin)
	: CScript((UINT)SCRIPT_TYPE::BOSSSCRIPT)
	, m_BossStatus(_Origin.m_BossStatus)
	, m_AttDuration(0.f)
	, m_EXsDuration(0.f)
	, m_ActiveAttack(false)
	, m_ActiveEXs(false)
	, m_EXsType(0)
	, m_Target(nullptr)
	, m_ArrMissile{}
	, m_ActiveShield(false)
{
	InitScriptParamUI();

	InitStateMachine();
}

CBossScript::~CBossScript()
{
	if (nullptr != m_FSM)
	{
		delete m_FSM;
		m_FSM = nullptr;
	}
}

void CBossScript::begin()
{
	m_FSM->Begin();
	m_FSM->SetCurState((int)BOSS_STATE::NormalIdle);

	m_Target = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Azusa");
}

void CBossScript::tick()
{
	m_FSM->Update();
	DebugState = magic_enum::enum_name((BOSS_STATE)m_FSM->GetCurState());

	Vec3 vDir = m_Target->Transform()->GetRelativePos() - Transform()->GetRelativePos();
	Transform()->SetDir(vDir);

	if ((int)BOSS_STATE::NormalIdle == m_FSM->GetCurState())
	{
		// 평타, 스킬 쿨타임 체크
		CheckDuration();
	}

	// Vital 상태
	CheckVital();
}

void CBossScript::CheckDuration()
{
	if (!m_ActiveAttack)
	{
		m_AttDuration += DT;

		if (m_AttDuration >= m_BossStatus.ATTSpeed)
		{
			m_AttDuration  = 0.f;
			m_ActiveAttack = true;
			// m_FSM->SetCurState((int)BOSS_STATE::NormalAttackStart);
		}
	}

	if (!m_ActiveEXs)
	{
		m_EXsDuration += DT;

		if (m_EXsDuration >= m_BossStatus.EXsCoolTime)
		{
			m_EXsDuration = 0.f;
			m_ActiveEXs	  = true;
		}
	}

	if (m_ActiveEXs)
	{
		// m_EXsType = CRandomMgr::GetInst()->GetRandomInt(4);
		m_EXsType = 2;
		switch (m_EXsType)
		{
		case 0:
			m_FSM->SetCurState((int)BOSS_STATE::EXs1);
			break;
		case 1:
			m_FSM->SetCurState((int)BOSS_STATE::EXs2);
			break;
		case 2:
			m_FSM->SetCurState((int)BOSS_STATE::EXs3);
			break;
		case 3:
			m_FSM->SetCurState((int)BOSS_STATE::EXs4);
			break;
		default:
			break;
		}
		m_ActiveEXs = false;
	}
	else if (m_ActiveAttack)
	{
		m_FSM->SetCurState((int)BOSS_STATE::NormalAttackStart);
		m_ActiveAttack = false;
	}
}

void CBossScript::CheckVital()
{
	if (m_BossStatus.CurHP <= 0.f)
		m_BossStatus.IsDead = true;

	if (m_BossStatus.IsGroggy && !m_BossStatus.IsDead)
		m_FSM->SetCurState((int)BOSS_STATE::VitalGroggy);
	else if (!m_BossStatus.IsGroggy && m_BossStatus.IsDead)
		m_FSM->SetCurState((int)BOSS_STATE::VitalDeath);
	else if (m_BossStatus.IsGroggy && m_BossStatus.IsDead)
		m_FSM->SetCurState((int)BOSS_STATE::VitalGroggyDeath);
}

void CBossScript::FireMegaFist()
{
	if (nullptr == m_Target)
		return;

	Vec3 HandBonePos = (Animator3D()->FindBoneMat(L"Bip001 R Hand_01") * Transform()->GetWorldMat()).Translation();

	CGameObject* megafist = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Boss\\Kaiten_Punch.pref")->Instantiate();
	megafist->GetScript<CMegaFistScript>()->InitMegaFistInfo(GetOwner(), m_Target, HandBonePos, 500.f, 500.f, 3.f, 10.f,
															 false, true);
	int layeridx = megafist->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(megafist, layeridx);
}

void CBossScript::FireBossMissile(int _idx)
{
	if (nullptr == m_Target)
		return;

	Vec3 MissileBonePos = {};

	switch (_idx)
	{
	case 167:
		MissileBonePos = (Animator3D()->FindBoneMat(L"Bone_Missile001") * Transform()->GetWorldMat()).Translation();
		break;
	case 169:
		MissileBonePos = (Animator3D()->FindBoneMat(L"Bone_Missile002") * Transform()->GetWorldMat()).Translation();
		break;
	case 173:
		MissileBonePos = (Animator3D()->FindBoneMat(L"Bone_Missile003") * Transform()->GetWorldMat()).Translation();
		break;
	case 177:
		MissileBonePos = (Animator3D()->FindBoneMat(L"Bone_Missile004") * Transform()->GetWorldMat()).Translation();
		break;
	case 180:
		MissileBonePos = (Animator3D()->FindBoneMat(L"Bone_Missile005") * Transform()->GetWorldMat()).Translation();
		break;
	case 182:
		MissileBonePos = (Animator3D()->FindBoneMat(L"Bone_Missile006") * Transform()->GetWorldMat()).Translation();
		break;
	case 186:
		MissileBonePos = (Animator3D()->FindBoneMat(L"Bone_Missile007") * Transform()->GetWorldMat()).Translation();
		break;
	case 190:
		MissileBonePos = (Animator3D()->FindBoneMat(L"Bone_Missile008") * Transform()->GetWorldMat()).Translation();
		break;
	default:
		break;
	}

	CGameObject* Missile = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Boss\\Kaiten_Missile.pref")->Instantiate();
	Missile->GetScript<CBossMissileScript>()->InitBossMissileInfo(GetOwner(), m_Target, MissileBonePos, 600.f, 1200.f,
																  3.f, 10.f, true, true);

	int layeridx = Missile->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(Missile, layeridx);
}

void CBossScript::ActiveShield()
{
	if (nullptr == m_Target)
		return;

	Vec3 ShieldPos = (Animator3D()->FindBoneMat(L"Bone_shield_03") * Transform()->GetWorldMat()).Translation();

	CGameObject* Shield = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Boss\\Kaiten_Shield.pref")->Instantiate();
	Shield->GetScript<CBossShieldScript>()->SetParent(GetOwner());

	int layeridx = Shield->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(Shield, layeridx);
}

void CBossScript::InitStateMachine()
{
	m_FSM = new CRoRStateMachine<CBossScript>(this, (UINT)BOSS_STATE::END);

	FSMInit(BOSS_STATE, CBossScript, NormalIdle);
	FSMInit(BOSS_STATE, CBossScript, NormalAttackStart);
	FSMInit(BOSS_STATE, CBossScript, NormalAttackIng);
	FSMInit(BOSS_STATE, CBossScript, NormalAttackDelay);
	FSMInit(BOSS_STATE, CBossScript, NormalAttackEnd);
	FSMInit(BOSS_STATE, CBossScript, EXs1);
	FSMInit(BOSS_STATE, CBossScript, EXs2);
	FSMInit(BOSS_STATE, CBossScript, EXs3);
	FSMInit(BOSS_STATE, CBossScript, EXs4);
	FSMInit(BOSS_STATE, CBossScript, VitalDeath);
	FSMInit(BOSS_STATE, CBossScript, VitalGroggy);
	FSMInit(BOSS_STATE, CBossScript, VitalGroggyDeath);
	FSMInit(BOSS_STATE, CBossScript, Ready1);
	FSMInit(BOSS_STATE, CBossScript, Ready2);
}

void CBossScript::InitScriptParamUI()
{
	AppendScriptParam("MaxHP       ", SCRIPT_PARAM::FLOAT, &m_BossStatus.MaxHP, 0.f);
	AppendScriptParam("CurHP       ", SCRIPT_PARAM::FLOAT, &m_BossStatus.CurHP, 0.f);
	AppendScriptParam("AttackDamage", SCRIPT_PARAM::FLOAT, &m_BossStatus.ATTDamage, 0.f);
	AppendScriptParam("AttackSpeed ", SCRIPT_PARAM::FLOAT, &m_BossStatus.ATTSpeed, 0.f);
	AppendScriptParam("EXsCoolTime ", SCRIPT_PARAM::FLOAT, &m_BossStatus.EXsCoolTime, 0.f);
	AppendScriptParam("EXsType     ", SCRIPT_PARAM::INT, &m_EXsType, 0, 3);

	AppendScriptParam("Dead        ", SCRIPT_PARAM::BOOL, &m_BossStatus.IsDead);
	AppendScriptParam("Groggy      ", SCRIPT_PARAM::BOOL, &m_BossStatus.IsGroggy);
}
