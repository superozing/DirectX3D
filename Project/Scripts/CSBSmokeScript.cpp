#include "pch.h"
#include "CSBSmokeScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>

CSBSmokeScript::CSBSmokeScript()
	: CScript((UINT)SCRIPT_TYPE::SBSMOKESCRIPT)
	, m_Kaiten(nullptr)
	, m_Child{}
	, m_Module{}
	, m_vecModule{}
	, m_vDir{}
{
}

CSBSmokeScript::~CSBSmokeScript()
{
}

void CSBSmokeScript::begin()
{
	m_Module = ParticleSystem()->GetParticleModule();
	ParticleSystem()->SetModule(m_Module);

	// 카이텐져 찾기
	m_Kaiten = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Kaiten");

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

void CSBSmokeScript::tick()
{
	m_vDir = -(m_Kaiten->Transform()->GetWorldDir(DIR_TYPE::FRONT));

	m_Module.FixedDirection = m_vDir;

	ParticleSystem()->SetModule(m_Module);

	for (size_t i = 0; i < m_vecModule.size(); ++i)
	{
		m_vecModule[i].FixedDirection = m_vDir;
		m_Child[i]->ParticleSystem()->SetModule(m_vecModule[i]);
	}

	int finalIdx = m_Kaiten->Animator3D()->GetCurClipLength();
	int idx		 = m_Kaiten->Animator3D()->GetCurFrameIdx();

	if (idx > finalIdx - 10)
		ParticleSystem()->Stop();
}
