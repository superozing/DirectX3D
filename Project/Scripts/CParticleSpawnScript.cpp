#include "pch.h"
#include "CParticleSpawnScript.h"

CParticleSpawnScript::CParticleSpawnScript()
	: CScript((UINT)SCRIPT_TYPE::PARTICLESPAWNSCRIPT)
	, m_Pos{}
	, m_AccTime(0.f)
	, m_Duration(0.f)
	, m_Repeat(false)
{
}

CParticleSpawnScript::CParticleSpawnScript(const CParticleSpawnScript& _Origin)
	: CScript((UINT)SCRIPT_TYPE::PARTICLESPAWNSCRIPT)
	, m_Pos{}
	, m_AccTime(0.f)
	, m_Duration(0.f)
	, m_Repeat(false)
{
}

CParticleSpawnScript::~CParticleSpawnScript()
{
}

void CParticleSpawnScript::begin()
{
	Transform()->SetRelativePos(m_Pos);

	ParticleSystem()->Play();
}

void CParticleSpawnScript::tick()
{
	m_Duration += DT;

	if (m_AccTime <= m_Duration)
	{
		GamePlayStatic::DestroyGameObject(GetOwner());
	}
}

void CParticleSpawnScript::SetParticleInfo(Vec3 _Pos, float _time, bool _Repeat)
{
	m_Pos	  = _Pos;
	m_AccTime = _time;
	m_Repeat  = _Repeat;
}