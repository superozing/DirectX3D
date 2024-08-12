#include "pch.h"
#include "CTurretBulletLine.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include "CTurret.h"

CTurretBulletLine::CTurretBulletLine()
	: CScript((UINT)SCRIPT_TYPE::TURRETBULLETLINE)
	, m_Parent(nullptr)
	, m_TargetPos{}
	, m_vDir{}
	, m_Module{}
	, m_bParticle(false)
	, m_offset(0.f, 0.f, 0.f)
	, m_offsetDir(0.f, 0.f, 0.f)
	, m_CurFrame(0)
	, m_PrevFrame(0)
	, fParticleTimer(2.f)
{
	AppendScriptParam("Offset", SCRIPT_PARAM::VEC3, &m_offset);
	AppendScriptParam("OffsetDir", SCRIPT_PARAM::VEC3, &m_offsetDir);
}

CTurretBulletLine::~CTurretBulletLine()
{
}

void CTurretBulletLine::begin()
{
	m_Module = ParticleSystem()->GetParticleModule();
	ParticleSystem()->SetModule(m_Module);
	ParticleSystem()->Play();

	m_Parent	  = GetOwner()->GetParent();
	Vec3 ShootPos = m_Parent->GetScript<CTurret>()->GetPShootPos();
	m_TargetPos	  = ShootPos;
}

void CTurretBulletLine::tick()
{
	if (m_Parent->GetScript<CTurret>()->IsDeadMonster())
	{
		ParticleSystem()->Stop();
		GamePlayStatic::DestroyGameObject(GetOwner());
	}

	// 가운데 위치 설정
	Vec3 vPos =
		(m_Parent->Animator3D()->FindBoneMat(L"bone_robot_gun_L") * m_Parent->Transform()->GetWorldMat()).Translation();
	vPos += m_offset;
	vPos.z += 100.f;
	vPos.y += 80.f;
	vPos.x -= 50.f;

	m_TargetPos.y = vPos.y;
	Transform()->SetRelativePos(vPos);

	m_Module.AddVelocityType = 2;
	m_vDir					 = (m_TargetPos - vPos).Normalize();
	m_Module.FixedDirection	 = m_vDir;
	m_Module.SpawnRate		 = 3;
	fParticleTimer -= DT;

	if (fParticleTimer < 0.f)
	{
		GamePlayStatic::DestroyGameObject(GetOwner());
	}

	ParticleSystem()->SetModule(m_Module);
}
