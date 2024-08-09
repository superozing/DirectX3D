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
	AppendScriptParam("Cur HP", SCRIPT_PARAM::INT, &m_CurHP);
	AppendScriptParam("Max HP", SCRIPT_PARAM::INT, &m_MaxHP);

	AppendScriptParam("Damage Value", SCRIPT_PARAM::INT, &takedamagetemp);
	AppendMemberFunction("TakeDamage", SCRIPT_PARAM::FUNC_MEMBER, "Call TakeDamage(Damage Value)",
						 std::bind(&CMonsterScript::DbgTakeDamage, this));
}

void CMonsterScript::tick()
{
}

void CMonsterScript::TakeDamage(int _Dmg)
{
	m_CurHP = RoRMath::ClampInt(m_CurHP - _Dmg, 0);
	
	if (m_CurHP <= 0)
		m_IsDeadMonster = true;
	else 
		m_IsDeadMonster = false;
}

void CMonsterScript::DbgTakeDamage()
{
	TakeDamage(takedamagetemp);
}


#define TagMonsterMaxHP "[Monster Max HP]"

void CMonsterScript::SaveToFile(FILE* _File)
{
}

void CMonsterScript::SaveToFile(ofstream& fout)
{
	fout << TagMonsterMaxHP << endl;
	fout << m_MaxHP << endl;
}

void CMonsterScript::LoadFromFile(FILE* _File)
{
}

void CMonsterScript::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagMonsterMaxHP);
	fin >> m_MaxHP;
}
