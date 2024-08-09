#include "pch.h"
#include "CBossScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CRandomMgr.h>
#include <Engine\CAssetMgr.h>

#include "CRoRStateMachine.h"
#include "CMegaFistScript.h"
#include "CMiniGunScript.h"
#include "CBossMissileScript.h"
#include "CBossShieldScript.h"
#include "CBossSmokeWaveScript.h"
#include "CBossBulletShellSpawner.h"
#include "CBossSwordBeamScript.h"
#include "CBulletLineScript.h"

static string DebugState = "";

CBossScript::CBossScript()
	: CScript((UINT)SCRIPT_TYPE::BOSSSCRIPT)
	, m_BossStatus{}
	, m_AttDuration(0.f)
	, m_EXsDuration(0.f)
	, m_ChaseDir(true)
	, m_ActiveAttack(false)
	, m_ActiveEXs(false)
	, m_EXsType(0)
	, m_Target(nullptr)
	, m_TargetPos{}
	, m_ArrMissile{}
	, m_ArrShield{}
	, m_bSwordBeam(false)
	, m_hitInfo{}
	, m_BulletInterval(0.f)
	, m_Raycast(false)
	, m_RaycastInterval(0.f)
{
	AppendScriptParam("CurState    ", SCRIPT_PARAM::STRING, &DebugState);
	AppendScriptParam("raycast", SCRIPT_PARAM::BOOL, &m_Raycast);
	InitScriptParamUI();

	InitStateMachine();
}

CBossScript::CBossScript(const CBossScript& _Origin)
	: CScript((UINT)SCRIPT_TYPE::BOSSSCRIPT)
	, m_BossStatus(_Origin.m_BossStatus)
	, m_AttDuration(0.f)
	, m_EXsDuration(0.f)
	, m_ChaseDir(true)
	, m_ActiveAttack(false)
	, m_ActiveEXs(false)
	, m_EXsType(0)
	, m_Target(nullptr)
	, m_TargetPos{}
	, m_ArrMissile{}
	, m_ArrShield{}
	, m_bSwordBeam(false)
	, m_hitInfo{}
	, m_BulletInterval(0.f)
	, m_Raycast(false)
	, m_RaycastInterval(0.f)
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

bool CBossScript::IsVital()
{
	int State = m_FSM->GetCurState();

	if ((int)BOSS_STATE::VitalGroggy == State || (int)BOSS_STATE::VitalDeath == State ||
		(int)BOSS_STATE::VitalGroggyDeath == State)
		return true;

	return false;
}

void CBossScript::begin()
{
	LoadAsset();

	m_FSM->Begin();
	m_FSM->SetCurState((int)BOSS_STATE::NormalIdle);

	m_Target = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Azusa");

	m_BulletShell = new CBossBulletShellSpawner;
	GetOwner()->AddComponent(m_BulletShell);
}

void CBossScript::tick()
{
	m_FSM->Update();
	DebugState = magic_enum::enum_name((BOSS_STATE)m_FSM->GetCurState());

	if (m_ChaseDir)
	{
		Vec3 vDir = m_Target->Transform()->GetRelativePos() - Transform()->GetRelativePos();
		Transform()->SetDir(vDir);
	}

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
			m_FSM->SetCurState((int)BOSS_STATE::NormalAttackStart);
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
		// m_EXsType = 0;
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
	if (!m_BossStatus.IsDead && !m_BossStatus.IsGroggy && m_BossStatus.CurHP > 0.f && m_BossStatus.CurHP <= 500.f)
		m_BossStatus.IsGroggy = true;

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

	CGameObject* megafist = CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_Punch)->Instantiate();
	megafist->GetScript<CMegaFistScript>()->InitMegaFistInfo(GetOwner(), m_Target, HandBonePos, 500.f, 500.f, 3.f, 10.f,
															 false, true);
	int layeridx = megafist->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(megafist, layeridx);
}

void CBossScript::FireMiniGun()
{
	if (nullptr == m_Target)
		return;

	CGameObject* pMinigun = CAssetMgr::GetInst()->Load<CPrefab>(PREFp_Minigun)->Instantiate();
	pMinigun->GetScript<CMiniGunScript>()->SetParent(GetOwner());

	int layeridx = pMinigun->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(pMinigun, layeridx);

	pMinigun = CAssetMgr::GetInst()->Load<CPrefab>(PREFp_BulletLine)->Instantiate();
	pMinigun->GetScript<CBulletLineScript>()->SetParent(GetOwner());
	pMinigun->GetScript<CBulletLineScript>()->SetTarget(m_Target);

	layeridx = pMinigun->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(pMinigun, layeridx);
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

	CGameObject* Missile = CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_Missile)->Instantiate();
	Missile->GetScript<CBossMissileScript>()->InitBossMissileInfo(GetOwner(), m_Target, MissileBonePos, 600.f, 1200.f,
																  3.f, 10.f, true, true);

	int layeridx = Missile->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(Missile, layeridx);
}

void CBossScript::ActiveInnerShield()
{
	if (nullptr == m_Target)
		return;

	Vec3 ShieldPos = (Animator3D()->FindBoneMat(L"Bone_shield_03") * Transform()->GetWorldMat()).Translation();

	CGameObject* Shield = CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_Shield)->Instantiate();
	Shield->GetScript<CBossShieldScript>()->SetParent(GetOwner());
	Shield->GetScript<CBossShieldScript>()->SetShieldType(SHIELD_TYPE::InnerShield);

	int layeridx = Shield->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(Shield, layeridx);
}

void CBossScript::ActiveOutsideShield()
{
	if (nullptr == m_Target)
		return;

	Vec3 ShieldPos = (Animator3D()->FindBoneMat(L"Bone_shield_03") * Transform()->GetWorldMat()).Translation();

	CGameObject* Shield = CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_Shield2)->Instantiate();
	Shield->GetScript<CBossShieldScript>()->SetParent(GetOwner());
	Shield->GetScript<CBossShieldScript>()->SetShieldType(SHIELD_TYPE::OutsideShield);

	int layeridx = Shield->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(Shield, layeridx);

	// SmokeWave 스폰
	CGameObject* SmokeWave = CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_SmokeWave)->Instantiate();

	layeridx = SmokeWave->GetLayerIdx();
	GetOwner()->AddChild(SmokeWave, true);
	GamePlayStatic::SpawnGameObject(SmokeWave, layeridx);
}

void CBossScript::ActiveHexShield()
{
	if (nullptr == m_Target)
		return;

	CGameObject* Shield = CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_HexShield)->Instantiate();
	Shield->GetScript<CBossShieldScript>()->SetParent(GetOwner());
	Shield->GetScript<CBossShieldScript>()->SetShieldType(SHIELD_TYPE::HexShield);

	int layeridx = Shield->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(Shield, layeridx);
}

void CBossScript::DeActiveHexShield()
{
	CGameObject* pObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Kaiten_HexShield");

	if (nullptr != pObj)
		GamePlayStatic::DestroyGameObject(pObj);
}

void CBossScript::ActiveSwordTrail()
{
	if (nullptr == m_Target)
		return;

	CGameObject* SwordTrail = CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_Slash)->Instantiate();
	int			 layeridx	= SwordTrail->GetLayerIdx();
	GetOwner()->AddChild(SwordTrail, true);
	GamePlayStatic::SpawnGameObject(SwordTrail, layeridx);
}

void CBossScript::FireSwordBeam()
{
	if (nullptr == m_Target)
		return;

	Vec3 HandBonePos = (Animator3D()->FindBoneMat(L"Bip001 R Hand_01") * Transform()->GetWorldMat()).Translation();

	CGameObject* SwordBeam = CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_SwordBeam)->Instantiate();
	SwordBeam->GetScript<CBossSwordBeamScript>()->InitSwordBeamInfo(GetOwner(), m_Target, m_TargetPos, HandBonePos,
																	5000.f, 5000.f, 3.f, 50.f, false, true);
	int layeridx = SwordBeam->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(SwordBeam, layeridx);
}

void CBossScript::ActiveWarningDecal()
{
	Vec3 vPos = Transform()->GetWorldPos();

	Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	CGameObject* pDecal = CAssetMgr::GetInst()->Load<CPrefab>(PREFSwordBeam_WarningDecal)->Instantiate();
	pDecal->Transform()->SetDir(-vDir);
	pDecal->Transform()->SetRelativePos(Vec3(m_TargetPos.x, m_TargetPos.y - 5.f, m_TargetPos.z));

	int layeridx = pDecal->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(pDecal, layeridx);
}

void CBossScript::LoadAsset()
{
	CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_Punch);
	CAssetMgr::GetInst()->Load<CPrefab>(PREFp_Minigun);
	CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_Missile);
	CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_Shield);
	CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_Shield2);
	CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_HexShield);
	CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_SmokeWave);
	CAssetMgr::GetInst()->Load<CPrefab>(PREFp_Explode);
	CAssetMgr::GetInst()->Load<CPrefab>(PREFp_Explode_Lite);
	CAssetMgr::GetInst()->Load<CPrefab>(PREFp_Electric);
	CAssetMgr::GetInst()->Load<CPrefab>(PREFBoss_Bullet_Shell);
	CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_Slash);
	CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_SwordBeam);
	CAssetMgr::GetInst()->Load<CPrefab>(PREFSwordBeam_WarningDecal);
}

void CBossScript::CheckTargetPos()
{
	if (nullptr == m_Target)
		return;

	m_TargetPos = m_Target->Transform()->GetRelativePos();
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
