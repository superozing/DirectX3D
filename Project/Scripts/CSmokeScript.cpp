#include "pch.h"
#include "CSmokeScript.h"

#include <Engine\CGameObject.h>

CSmokeScript::CSmokeScript()
	: CScript((UINT)SCRIPT_TYPE::SMOKESCRIPT)
	, m_Kaiten(nullptr)
	, m_Child{}
	, m_Module{}
	, m_vecModule{}
	, m_vDir{}
{
}

CSmokeScript::~CSmokeScript()
{
}

void CSmokeScript::begin()
{
	m_Module = ParticleSystem()->GetParticleModule();
	ParticleSystem()->SetModule(m_Module);

	// 카이텐져 찾기
	if (GetOwner()->GetParent() && GetOwner()->GetParent()->GetParent())
	{
		m_Kaiten = GetOwner()->GetParent()->GetParent();
	}

	m_Child = GetOwner()->GetChild();
	if (!m_Child.empty())
	{
		for (auto child : m_Child)
		{
			tParticleModule module = child->ParticleSystem()->GetParticleModule();
			m_vecModule.push_back(module);
		}
	}

	ParticleSystem()->Play();
}

void CSmokeScript::tick()
{
	m_vDir = -(m_Kaiten->Transform()->GetWorldDir(DIR_TYPE::FRONT));

	m_Module.FixedDirection = m_vDir;

	ParticleSystem()->SetModule(m_Module);

	for (size_t i = 0; i < m_vecModule.size(); ++i)
	{
		m_vecModule[i].FixedDirection = m_vDir;
		m_Child[i]->ParticleSystem()->SetModule(m_vecModule[i]);
	}

	int idx = m_Kaiten->Animator3D()->GetCurFrameIdx();

	if (idx > 155)
		ParticleSystem()->Stop();
}
