#include "pch.h"
#include "CMonsterScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CMonsterScript::CMonsterScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT)
{
}

CMonsterScript::CMonsterScript(const CMonsterScript& _Origin)
	: CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT)
{
}

CMonsterScript::~CMonsterScript()
{
}

static int takedamagetemp;

void CMonsterScript::begin()
{
	AppendScriptParam("Cur Health", SCRIPT_PARAM::INT, &m_tStatus.CurHealth);
	AppendScriptParam("Max Health", SCRIPT_PARAM::INT, &m_tStatus.MaxHealth);
	AppendScriptParam("IsDead", SCRIPT_PARAM::BOOL, &m_tStatus.IsDead, 0, 0, true);
	AppendScriptParam("IsInvincibility", SCRIPT_PARAM::BOOL, &m_tStatus.Invincibility, 0, 0, true);

	AppendScriptParam("Damage", SCRIPT_PARAM::FLOAT, &m_tStatus.Damage);
	AppendScriptParam("Defensive", SCRIPT_PARAM::FLOAT, &m_tStatus.Defensive);
	AppendScriptParam("Avoid Rate", SCRIPT_PARAM::FLOAT, &m_tStatus.AvoidPercent);
	AppendScriptParam("Critical Rate", SCRIPT_PARAM::FLOAT, &m_tStatus.CriticalPercent);
	AppendScriptParam("Critical Damage", SCRIPT_PARAM::FLOAT, &m_tStatus.CriticalDamage);
	AppendScriptParam("MoveSpeed", SCRIPT_PARAM::FLOAT, &m_tStatus.MoveSpeed);
	AppendScriptParam("AttackMoveSpeed", SCRIPT_PARAM::FLOAT, &m_tStatus.AttackMoveSpeed);

	AppendScriptParam("Take Damage Value", SCRIPT_PARAM::INT, &takedamagetemp);
	AppendMemberFunction("TakeDamage()", SCRIPT_PARAM::FUNC_MEMBER, "Call TakeDamage(Damage Value)",
						 std::bind(&CMonsterScript::DbgTakeDamage, this));
}

void CMonsterScript::tick()
{
}

void CMonsterScript::TakeDamage(int _Dmg)
{
	m_tStatus.CurHealth = RoRMath::ClampInt(m_tStatus.CurHealth - _Dmg, 0);
	
	if (m_tStatus.CurHealth <= 0)
		m_tStatus.IsDead = true;
	else 
		m_tStatus.IsDead = false;
}

void CMonsterScript::DbgTakeDamage()
{
	TakeDamage(takedamagetemp);
}


#define TagMonsterMaxHP				"[Monster Max HP]"
#define TagMonsterDamage			"[Monster Damage]"
#define TagMonsterDefensive			"[Monster Defensive]"
#define TagMonsterMoveSpeed			"[Monster MoveSpeed]"
#define TagMonsterRotateSpeed		"[Monster RotateSpeed]"
#define TagMonsterAvoidPercent		"[Monster AvoidPercent]"
#define TagMonsterCriticalPercent	"[Monster CriticalPercent]"
#define TagMonsterCriticalDamage	"[Monster CriticalDamage]"

void CMonsterScript::SaveToFile(FILE* _File)
{
}

void CMonsterScript::SaveToFile(ofstream& fout)
{
	fout << TagMonsterMaxHP << endl;
	fout << m_tStatus.MaxHealth << endl;

	fout << TagMonsterDamage << endl;
	fout << m_tStatus.Damage << endl;

	fout << TagMonsterDefensive << endl;
	fout << m_tStatus.Defensive << endl;

	fout << TagMonsterMoveSpeed << endl;
	fout << m_tStatus.MoveSpeed << endl;

	fout << TagMonsterRotateSpeed << endl;
	fout << m_tStatus.RotateSpeed << endl;

	fout << TagMonsterAvoidPercent << endl;
	fout << m_tStatus.AvoidPercent << endl;

	fout << TagMonsterCriticalPercent << endl;
	fout << m_tStatus.CriticalPercent << endl;

	fout << TagMonsterCriticalDamage << endl;
	fout << m_tStatus.CriticalDamage << endl;
}

void CMonsterScript::LoadFromFile(FILE* _File)
{
}

void CMonsterScript::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagMonsterMaxHP);
	fin >> m_tStatus.MaxHealth;

	Utils::GetLineUntilString(fin, TagMonsterDamage);
	fin >> m_tStatus.Damage;

	Utils::GetLineUntilString(fin, TagMonsterDefensive);
	fin >> m_tStatus.Defensive;

	Utils::GetLineUntilString(fin, TagMonsterMoveSpeed);
	fin >> m_tStatus.MoveSpeed;

	Utils::GetLineUntilString(fin, TagMonsterRotateSpeed);
	fin >> m_tStatus.RotateSpeed;

	Utils::GetLineUntilString(fin, TagMonsterAvoidPercent);
	fin >> m_tStatus.AvoidPercent;

	Utils::GetLineUntilString(fin, TagMonsterCriticalPercent);
	fin >> m_tStatus.CriticalPercent;

	Utils::GetLineUntilString(fin, TagMonsterCriticalDamage);
	fin >> m_tStatus.CriticalDamage;
}
