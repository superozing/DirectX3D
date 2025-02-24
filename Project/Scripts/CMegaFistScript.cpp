﻿#include "pch.h"
#include "CMegaFistScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\CRandomMgr.h>
#include <Engine\CPhysXMgr.h>
#include <Engine\CPhysX.h>

#include "CPlayerScript.h"
#include "CBossScript.h"
#include "CRoRStateMachine.h"
#include "CDamagedDirectionMgr.h"

static string shooter = "";
static string target  = "";

CMegaFistScript::CMegaFistScript()
	: CProjectileScript((UINT)SCRIPT_TYPE::MEGAFISTSCRIPT)
	, m_Shooter(nullptr)
	, m_Target(nullptr)
	, m_TargetPos(0.f, 0.f, 0.f)
	, m_Gravity(0.f, 0.f, 0.f)
	, m_bPhysX(false)
	, m_bHit(false)
{
}

CMegaFistScript::~CMegaFistScript()
{
}

void CMegaFistScript::begin()
{
	CProjectileScript::begin();

	Transform()->SetRelativePos(m_Pos);

	m_Gravity.y = CPhysXMgr::GetInst()->GetGravity();
	float PPM	= CPhysXMgr::GetInst()->GetPPM();

	m_Gravity.y /= PPM;

	// 수평거리
	float dx		   = m_TargetPos.x - m_Pos.x;
	float dz		   = m_TargetPos.z - m_Pos.z;
	float d_horizontal = std::sqrt(dx * dx + dz * dz);

	// 수직거리
	float dy = m_TargetPos.y - m_Pos.y;

	// 초기속도
	float v = std::sqrt((m_Gravity.y / (2 * dy)) * (d_horizontal * d_horizontal));

	Vec3 VelDir = m_TargetPos - m_Pos;
	VelDir.y	= 0.f;
	VelDir.Normalize();
	m_CurVelocity = VelDir * v;
	m_bPhysX	  = false;

	float colTime = d_horizontal / v;

	m_AngleVelocity = Vec3(-90.f, 0.f, 0.f) / colTime;
}

void CMegaFistScript::tick()
{
	CProjectileScript::tick();

	if (m_Shooter->GetScript<CBossScript>()->IsVital())
	{
		GamePlayStatic::DestroyGameObject(GetOwner());
	}

	if (m_IsAlive)
	{
		m_LifeSpan -= DT;

		if (m_LifeSpan <= 0.f)
			GamePlayStatic::DestroyGameObject(GetOwner());
	}
	else
		GamePlayStatic::DestroyGameObject(GetOwner());

	if (!m_bPhysX)
	{
		PhysX()->setLinearVelocity(m_CurVelocity);
		PhysX()->setAngularVelocity(m_AngleVelocity);
		m_bPhysX = true;
	}

	// m_CurVelocity += m_Gravity * DT;
	// Transform()->SetDir(m_CurVelocity);

	// m_Pos = Transform()->GetRelativePos();
	// m_Pos = m_Pos + m_CurVelocity * DT;
	// Transform()->SetRelativePos(m_Pos);
}

void CMegaFistScript::OnHit()
{
	m_Target->GetScript<CPlayerScript>()->SetDamagedMove(true);
	m_Target->GetScript<CPlayerScript>()->Hit(m_Damage);
	m_BossScript->GetDmgDirMgr()->AddDamagedDirection(m_Shooter->Transform()->GetWorldPos(), 0.2f);
}

void CMegaFistScript::InitMegaFistInfo(CGameObject* _Shooter, CGameObject* _Target, Vec3 _Pos, float _InitSpeed,
									   float _MaxSpeed, float _LifeSpan, float _Damage, bool _Explode, bool _Alive)
{
	m_Shooter = _Shooter;
	m_Target  = _Target;
	m_BossScript = _Shooter->GetScript<CBossScript>();
	CProjectileScript::InitProjectileInfo(_Pos, _InitSpeed, _MaxSpeed, _LifeSpan, _Damage, _Explode, _Alive);

	// 타겟 위치 기준 바닥보다 조금 더 위를 노리도록 설정
	m_TargetPos = m_Target->Transform()->GetRelativePos();
	m_TargetPos.y += 50.f;
}

void CMegaFistScript::BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
	if ((int)LAYER::LAYER_PLAYER == _OtherObj->GetLayerIdx() && !m_bHit)
	{
		OnHit();
	}

	if (!m_bHit)
		m_Shooter->GetScript<CBossScript>()->GetSound((UINT)BOSS_SOUND::NORMAL_HIT)->Play(1.f, 1.f);

	m_bHit = true;

	// m_IsAlive = false;
}

void CMegaFistScript::Overlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
}

void CMegaFistScript::EndOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
}