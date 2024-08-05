#include "pch.h"
#include "CMissileTrailScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>


CMissileTrailScript::CMissileTrailScript()
	: CScript((UINT)SCRIPT_TYPE::MISSILETRAILSCRIPT)
	, m_Parent(nullptr)
	, m_vDir{}
	, m_Module{}
{
}

CMissileTrailScript::~CMissileTrailScript()
{
}


void CMissileTrailScript::begin()
{
	m_Module = ParticleSystem()->GetParticleModule();
	ParticleSystem()->SetModule(m_Module);
	if (GetOwner()->GetParent())
	{
		m_Parent = GetOwner()->GetParent();
	}

	ParticleSystem()->Play();
}

void CMissileTrailScript::tick()
{
	m_vDir = -(m_Parent->Transform()->GetWorldDir(DIR_TYPE::FRONT));

	m_Module.FixedDirection = m_vDir;

	ParticleSystem()->SetModule(m_Module);
}