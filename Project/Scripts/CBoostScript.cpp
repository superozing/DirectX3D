#include "pch.h"
#include "CBoostScript.h"

CBoostScript::CBoostScript()
	: CScript((UINT)SCRIPT_TYPE::BOOSTSCRIPT)
	, m_Parent(nullptr)
	, m_vDir{}
	, m_Module{}
{
}

CBoostScript::~CBoostScript()
{
}


void CBoostScript::begin()
{
	m_Module = ParticleSystem()->GetParticleModule();
	ParticleSystem()->SetModule(m_Module);
	if (GetOwner()->GetParent())
	{
		m_Parent = GetOwner()->GetParent();
	}

	ParticleSystem()->Play();
}

void CBoostScript::tick()
{
	m_vDir = -(m_Parent->Transform()->GetWorldDir(DIR_TYPE::FRONT));

	m_Module.FixedDirection = m_vDir;

	ParticleSystem()->SetModule(m_Module);
}