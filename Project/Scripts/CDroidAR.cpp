#include "pch.h"
#include "CDroidAR.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CRandomMgr.h>
#include <Engine\CAssetMgr.h>

#include "CMuzzleFlash_DroidAR.h"
#include "CShootingSystem_DroidAR.h"
#include "CRoRStateMachine.h"

CDroidAR::CDroidAR()
	: CMonsterScript((UINT)SCRIPT_TYPE::DROIDAR)
{
	InitScriptParamUI();
	InitStateMachine();
}

CDroidAR::CDroidAR(const CDroidAR& _Origin)
	: CMonsterScript((UINT)SCRIPT_TYPE::DROIDAR)
{
	InitScriptParamUI();
	InitStateMachine();

	// 구현하기
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

	// 제일 첫 상태로 들어가게 됨. (다른 설정을 해주지 않으면)
	m_FSM->Begin();
	
	// 첫 상태를 지정
	m_FSM->SetCurState((int)DROIDAR_STATE::MoveIng);

	m_Target = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Azusa");

	m_AttackCount =	m_MaxAttackCount;

	auto pObj	  = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab/Monster/p_MuzzleFlash_DroidAR.pref")->Instantiate();
	m_MuzzleFlash = pObj->GetScript<CMuzzleFlash_DroidAR>();
						
	m_MuzzleFlash->SetDroid(GetOwner());

	GetOwner()->AddChild(pObj, true);

	m_ShootingSystem = GetOwner()->GetScript<CShootingSystem_DroidAR>();
}

void CDroidAR::tick()
{
	m_FSM->Update();

	// 아무 것도 안 하고 있는 상태일 경우
	if ((int)DROIDAR_STATE::NormalIdle == m_FSM->GetCurState())
	{
		// 사격 범위 내에 있는지 체크
		if (CheckTargetInRange())
		{ 
			// 공격 쿨타임 체크
			CheckAttackCooldown();
		}
		else if (m_FSM->GetCurState() != (int)DROIDAR_STATE::MoveIng)
		{
			// 너무 멀리 있다면 플레이어의 위치로 이동
			m_FSM->SetCurState((int)DROIDAR_STATE::MoveIng);
		}
	}

	// Vital 상태
	CheckVital();
}

void CDroidAR::LoadAsset()
{
	//CAssetMgr::GetInst()->Load<CPrefab>(PREFKaiten_Punch);
}

void CDroidAR::CheckAttackCooldown()
{
	// 공격 중이 아닌 경우
	if (!m_IsActiveAttack)
	{
		// 공격 대기 시간 누적하기
		m_AttCooldown += DT;

		// 공격 대기 시간을 넘어갈 경우 공격 state로 전환
		if (m_AttCooldown >= 1.f) // m_MonsterStatus.AttackMoveSpeed
		{
			m_AttCooldown	 = 0.f;
			m_IsActiveAttack = true;
		}
	}
	
	// 공격 중인 경우
	if (m_IsActiveAttack)
	{
		m_FSM->SetCurState((int)DROIDAR_STATE::NormalAttackStart);
		m_IsActiveAttack = false;
	}
}

void CDroidAR::CheckVital()
{
	// 현재 hp가 0보다 적을 경우
	if (m_MonsterStatus.CurHealth <= 0.f)
	{
		// dead 상태로 전환
		m_MonsterStatus.IsDead = true;
		m_FSM->SetCurState((int)DROIDAR_STATE::VitalDeath);
	}
}

bool CDroidAR::CheckTargetInRange()
{
	//distance가 범위 내에 있는지 체크
	Vec3 PlayerPos = m_Target->Transform()->GetWorldPos();
	Vec3 DroidPos = GetOwner()->Transform()->GetWorldPos();

	float dist = Vec3::Distance(DroidPos, PlayerPos);

	return dist < 1000.f;
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

void CDroidAR::RotationDirectionToPlayer()
{
	Vec3 PlayerPos = m_Target->Transform()->GetWorldPos();
	Vec3 DroidPos  = GetOwner()->Transform()->GetWorldPos();

	Vec3 dir = PlayerPos - DroidPos;
	GetOwner()->Transform()->SetRelativePos(DroidPos + (dir.Normalize() * 200.f * DT));
	dir.y = 0.f;
	GetOwner()->Transform()->SetDir(dir.Normalize());
}
