#include "pch.h"
#include "CProjectileScript.h"

CProjectileScript::CProjectileScript(UINT _ScriptType)
	: CScript(_ScriptType)
	, m_Pos(0.f, 0.f, 0.f)
	, m_InitialSpeed(0.f)
	, m_MaxSpeed(0.f)
	, m_LifeSpan(0.f)
	, m_Damage(0.f)
	, m_Explode(false)
	, m_IsAlive(false)
{
}

CProjectileScript::~CProjectileScript()
{
}

void CProjectileScript::begin()
{
	InitProjectileScriptParamUI();
}

void CProjectileScript::tick()
{
}

void CProjectileScript::OnHit()
{
}

void CProjectileScript::InitProjectileInfo(Vec3 _Pos, float _InitSpeed, float _MaxSpeed, float _LifeSpan, float _Damage,
										   bool _Explode, bool _Alive)
{
	m_Pos		   = _Pos;
	m_InitialSpeed = _InitSpeed;
	m_MaxSpeed	   = _MaxSpeed;
	m_LifeSpan	   = _LifeSpan;
	m_Damage	   = _Damage;
	m_Explode	   = _Explode;
	m_IsAlive	   = _Alive;
}

void CProjectileScript::InitProjectileScriptParamUI()
{
	AppendScriptParam("Position  ", SCRIPT_PARAM::VEC3, &m_Pos);
	AppendScriptParam("Init Speed", SCRIPT_PARAM::FLOAT, &m_InitialSpeed);
	AppendScriptParam("Max Speed ", SCRIPT_PARAM::FLOAT, &m_MaxSpeed);
	AppendScriptParam("LifeSpan  ", SCRIPT_PARAM::FLOAT, &m_LifeSpan);
	AppendScriptParam("Damage    ", SCRIPT_PARAM::FLOAT, &m_Damage);
	AppendScriptParam("Explode   ", SCRIPT_PARAM::BOOL, &m_Explode);
	AppendScriptParam("Alive     ", SCRIPT_PARAM::BOOL, &m_IsAlive);
}
