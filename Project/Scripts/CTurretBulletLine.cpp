#include "pch.h"
#include "CTurretBulletLine.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include "CTurret.h"
#include "CTurretShootingSystem.h"
// #include <Engine\CGameObject.h>

CTurretBulletLine::CTurretBulletLine()
	: CScript((UINT)SCRIPT_TYPE::TURRETBULLETLINE)
	, m_Parent(nullptr)
	, m_TargetPos{}
	, m_vDir{}
	, m_Module{}
	, m_bParticle(false)
	, m_offset(0.f, 0.f, 0.f)
	, m_offsetDir(0.f, 500.f, 0.f)
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
	m_Parent	  = GetOwner()->GetParent();
	Vec3 ShootPos = m_Parent->GetScript<CTurret>()->GetPShootPos();
	m_TargetPos	  = ShootPos;

	this->Transform()->SetAbsolute(false);

	// 원하는 월드 위치 계산
	Vec3 desiredWorldPos = m_Parent->Transform()->GetWorldPos();
	desiredWorldPos += m_offset;

	// 부모의 월드 변환의 역변환을 계산
	Matrix parentWorldInv = m_Parent->Transform()->GetWorldInvMat();

	// 원하는 월드 위치를 부모 기준의 로컬 위치로 변환
	Vec3 localPos = XMVector3Transform(desiredWorldPos, parentWorldInv);
	localPos.y += 0.5f;

	// 자식(this)의 상대적 위치 설정
	this->Transform()->SetRelativePos(localPos);

	auto shootingsys = m_Parent->GetScript<CTurretShootingSystem>();

	m_Module				 = ParticleSystem()->GetParticleModule();
	m_Module.AddVelocityType = 2;
	m_Module.FixedDirection	 = m_Parent->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	m_Module.SpawnRate		 = 3;

	ParticleSystem()->SetModule(m_Module);
	ParticleSystem()->Play();
}

void CTurretBulletLine::tick()
{
	if (m_Parent->GetScript<CTurret>()->IsDeadMonster())
	{
		ParticleSystem()->Stop();
		GamePlayStatic::DestroyGameObject(GetOwner());
	}

	// 가운데 위치 설정
	// Matrix vMat = m_Parent->Animator3D()->FindBoneMat(L"bone_robot_gun_L");
	// Transform()->SetFrameMat(vMat);

	fParticleTimer -= DT;

	if (fParticleTimer < 0.f)
	{
		GamePlayStatic::DestroyGameObject(GetOwner());
	}

	ParticleSystem()->SetModule(m_Module);
}
