#include "pch.h"
#include "CDroidAR.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CRandomMgr.h>
#include <Engine\CAssetMgr.h>

#include "CRoRStateMachine.h"
#include "CBossScript.h"

CDroidAR::CDroidAR()
{
	InitScriptParamUI();
	InitStateMachine();
}

CDroidAR::CDroidAR(const CDroidAR& _Origin)
{
	InitScriptParamUI();
	InitStateMachine();
}

CDroidAR::~CDroidAR()
{
	if (nullptr != m_FSM)
	{
		delete m_FSM;
		m_FSM = nullptr;
	}
}

void CDroidAR::begin()
{
	LoadAsset();

	m_FSM->Begin();
	m_FSM->SetCurState((int)DROIDAR_STATE::NormalIdle);

	m_Target = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Azusa");
}

void CDroidAR::tick()
{
	m_FSM->Update();
	DebugState = magic_enum::enum_name((BOSS_STATE)m_FSM->GetCurState());

	if (m_ChaseDir)
	{
		Vec3 vDir = m_Target->Transform()->GetRelativePos() - Transform()->GetRelativePos();
		Transform()->SetDir(vDir);
	}

	if ((int)DROIDAR_STATE::NormalIdle == m_FSM->GetCurState())
	{
		// 평타, 스킬 쿨타임 체크
		CheckDuration();
	}

	// Vital 상태
	CheckVital();
}

void CDroidAR::LoadAsset()
{
	//CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_Punch);
}

void CDroidAR::CheckDuration()
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

void CDroidAR::CheckVital()
{
	if (!m_BossStatus.IsDead && !m_BossStatus.IsGroggy && m_BossStatus.GroggyBar >= 100.f)
	{
		m_BossStatus.IsGroggy  = true;
		m_BossStatus.GroggyBar = 0.f;
	}

	if (m_BossStatus.CurHP <= 0.f)
		m_BossStatus.IsDead = true;

	if (m_BossStatus.IsGroggy && !m_BossStatus.IsDead)
		m_FSM->SetCurState((int)BOSS_STATE::VitalGroggy);
	else if (!m_BossStatus.IsGroggy && m_BossStatus.IsDead)
		m_FSM->SetCurState((int)BOSS_STATE::VitalDeath);
	else if (m_BossStatus.IsGroggy && m_BossStatus.IsDead)
		m_FSM->SetCurState((int)BOSS_STATE::VitalGroggyDeath);
}

void CDroidAR::InitStateMachine()
{
	m_FSM = new CRoRStateMachine<CDroidAR>(this, (UINT)DROIDAR_STATE::END);

	FSMInit(DROIDAR_STATE, CDroidAR, Appere);
	FSMInit(DROIDAR_STATE, CDroidAR, EXs);
	FSMInit(DROIDAR_STATE, CDroidAR, NormalIdle);
	FSMInit(DROIDAR_STATE, CDroidAR, NormalReload);
	FSMInit(DROIDAR_STATE, CDroidAR, NormalAttackStart);
	FSMInit(DROIDAR_STATE, CDroidAR, NormalAttackIng);
	FSMInit(DROIDAR_STATE, CDroidAR, NormalAttackDelay);
	FSMInit(DROIDAR_STATE, CDroidAR, NormalAttackEnd);
	FSMInit(DROIDAR_STATE, CDroidAR, MoveIng);
	FSMInit(DROIDAR_STATE, CDroidAR, MoveEnd);
	FSMInit(DROIDAR_STATE, CDroidAR, VitalPanic);
	FSMInit(DROIDAR_STATE, CDroidAR, VitalDeath);
}

void CDroidAR::InitScriptParamUI()
{

}
