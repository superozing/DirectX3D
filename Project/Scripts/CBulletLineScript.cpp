#include "pch.h"
#include "CBulletLineScript.h"

#include "CBossScript.h"

CBulletLineScript::CBulletLineScript()
	: CScript((UINT)SCRIPT_TYPE::BULLETLINESCRIPT)
	, m_Parent(nullptr)
	, m_Target(nullptr)
	, m_TargetPos{}
	, m_vDir{}
	, m_Module{}
	, m_bParticle(false)
	, m_offset(-10.f, -20.f, 100.f)
	, m_offsetDir(0.f, -0.2f, 0.f)
	, m_CurFrame(0)
	, m_PrevFrame(0)
{
	AppendScriptParam("Offset", SCRIPT_PARAM::VEC3, &m_offset);
	AppendScriptParam("OffsetDir", SCRIPT_PARAM::VEC3, &m_offsetDir);
}

CBulletLineScript::~CBulletLineScript()
{
}

void CBulletLineScript::begin()
{
	m_Module = ParticleSystem()->GetParticleModule();
	ParticleSystem()->SetModule(m_Module);
	ParticleSystem()->Stop();

	m_TargetPos = m_Target->Transform()->GetRelativePos();
}

void CBulletLineScript::tick()
{
	if (m_Parent->GetScript<CBossScript>()->IsVital())
	{
		ParticleSystem()->Stop();
		GamePlayStatic::DestroyGameObject(GetOwner());
	}

	Vec3 vPos = (m_Parent->Animator3D()->FindBoneMat(L"fire_01") * m_Parent->Transform()->GetWorldMat()).Translation();
	vPos += m_offset;
	Transform()->SetRelativePos(vPos);

	m_vDir					= m_TargetPos - vPos;
	m_Module.FixedDirection = m_vDir;

	m_CurFrame = m_Parent->Animator3D()->GetCurFrameIdx();

	if (m_bParticle && m_CurFrame != m_PrevFrame)
	{
		m_Module.SpawnRate -= 1;

		if (m_Module.SpawnRate <= 0)
			m_Module.SpawnRate = 0;

		m_PrevFrame = m_CurFrame;
	}

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

	ParticleSystem()->SetModule(m_Module);
}
