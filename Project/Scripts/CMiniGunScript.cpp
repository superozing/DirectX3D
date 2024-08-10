#include "pch.h"
#include "CMiniGunScript.h"

#include "CBossScript.h"
#include "CRoRStateMachine.h"

CMiniGunScript::CMiniGunScript()
	: CScript((UINT)SCRIPT_TYPE::MINIGUNSCRIPT)
	, m_Parent(nullptr)
	, m_vDir{}
	, m_Module{}
	, m_bParticle(false)
	, m_offset(-10.f, -20.f, 100.f)
	, m_offsetDir(0.f, -0.2f, 0.f)
{
	AppendScriptParam("Offset", SCRIPT_PARAM::VEC3, &m_offset);
	AppendScriptParam("OffsetDir", SCRIPT_PARAM::VEC3, &m_offsetDir);
}

CMiniGunScript::~CMiniGunScript()
{
}

void CMiniGunScript::begin()
{
	m_Module = ParticleSystem()->GetParticleModule();
	ParticleSystem()->SetModule(m_Module);
	ParticleSystem()->Stop();
}

void CMiniGunScript::tick()
{
	if (m_Parent->GetScript<CBossScript>()->IsVital())
	{
		ParticleSystem()->Stop();
		GamePlayStatic::DestroyGameObject(GetOwner());
	}

	m_vDir = m_Parent->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	m_vDir += m_offsetDir;
	m_Module.FixedDirection = m_vDir;

	ParticleSystem()->SetModule(m_Module);
	Vec3 vPos = (m_Parent->Animator3D()->FindBoneMat(L"fire_01") * m_Parent->Transform()->GetWorldMat()).Translation();
	vPos += m_offset;
	Transform()->SetRelativePos(vPos);

	if (60 <= m_Parent->Animator3D()->GetCurFrameIdx() && !m_bParticle)
	{
		ParticleSystem()->Play();
		m_bParticle = true;
	}
	else if (123 <= m_Parent->Animator3D()->GetCurFrameIdx() && m_bParticle)
	{
		ParticleSystem()->Stop();
		GamePlayStatic::DestroyGameObject(GetOwner());
	}
}
