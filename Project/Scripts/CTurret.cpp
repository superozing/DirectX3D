#include "pch.h"
#include "CTurret.h"

#include "CRoRStateMachine.h"
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

static string DebugState = "";

CTurret::CTurret()
	: CMonsterScript((UINT)SCRIPT_TYPE::TURRET)

{

	SetTurretInfo();
	InitStateMachine();
	AppendScriptParam("CurState    ", SCRIPT_PARAM::STRING, &DebugState);
	AppendScriptParam("CurHP    ", SCRIPT_PARAM::INT, &m_tStatus.CurHealth);
}

CTurret::CTurret(const CTurret& _Origin)
	: CMonsterScript((UINT)SCRIPT_TYPE::TURRET)
{
	SetTurretInfo();
	InitStateMachine();
	AppendScriptParam("CurState    ", SCRIPT_PARAM::STRING, &DebugState);
	AppendScriptParam("CurHP    ", SCRIPT_PARAM::INT, &m_tStatus.CurHealth);
}

CTurret::~CTurret()
{
	if (nullptr != m_FSM)
	{
		delete m_FSM;
		m_FSM = nullptr;
	}
}

void CTurret::SetTurretInfo()
{
	int	  MaxHP	 = 150.f;
	float Damage = 10.f;

	SetMaxHP(MaxHP);
	SetCurHP(MaxHP);
	SetMonsterDamage(Damage);

	fDetectDistance = AttackDistance;
	fAttackTimer	= AttackTimer;
}

void CTurret::begin()
{
	m_FSM->Begin();
	m_FSM->SetCurState((int)TURRET_STATE::Appear);

	m_Target = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Azusa");
}

void CTurret::tick()
{
	m_FSM->Update();
	DebugState = magic_enum::enum_name((TURRET_STATE)m_FSM->GetCurState());

	if ((int)TURRET_STATE::NormalIdle == m_FSM->GetCurState())
	{
		fAttackTimer -= DT;
		CheckAttackable();
	}

	CheckVital();
}

void CTurret::SaveToFile(FILE* _File)
{
}

void CTurret::SaveToFile(ofstream& fout)
{
}

void CTurret::LoadFromFile(FILE* _File)
{
}

void CTurret::LoadFromFile(ifstream& fin)
{
}

void CTurret::CheckAttackable()
{
	float d = CalTargetDistance();

	if (fAttackTimer < 0.f && d <= fDetectDistance)
	{
		if (m_FSM->GetCurState() == (int)TURRET_STATE::NormalIdle)
		{
			m_FSM->SetCurState((int)TURRET_STATE::AttackStart);
		}
	}
}

float CTurret::CalTargetDistance()
{
	if (m_Target == nullptr)
		return 0.f;

	Vec3 targetPos = m_Target->Transform()->GetWorldPos();

	return Vec3::Distance(GetOwner()->Transform()->GetWorldPos(), targetPos);
}

void CTurret::CheckVital()
{
	if (this->GetCurHP() <= 0.f && this->IsDeadMonster() == true)
		m_FSM->SetCurState((int)TURRET_STATE::VitalDeath);
}