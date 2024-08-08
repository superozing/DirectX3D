﻿#include "pch.h"
#include "CBossSwordBeamScript.h"

CBossSwordBeamScript::CBossSwordBeamScript()
	: CProjectileScript((UINT)SCRIPT_TYPE::BOSSSWORDBEAMSCRIPT)
	, m_Shooter(nullptr)
	, m_Target(nullptr)
	, m_TargetPos(0.f, 0.f, 0.f)
	, m_Gravity(0.f, -981.f * 3.f, 0.f)
{
}

CBossSwordBeamScript::~CBossSwordBeamScript()
{
}

void CBossSwordBeamScript::begin()
{
	CProjectileScript::begin();

	Transform()->SetRelativePos(m_Pos);

	Vec3 VelDir = m_TargetPos - m_Pos;
	VelDir.y	= 0.f;
	VelDir.Normalize();
	m_CurVelocity = VelDir * m_InitialSpeed;

	Vec3 Dir = m_Shooter->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	Transform()->SetDir(Dir);
}

void CBossSwordBeamScript::tick()
{
	CProjectileScript::tick();

	if (m_IsAlive)
	{
		m_LifeSpan -= DT;

		if (m_LifeSpan <= 0.f)
			GamePlayStatic::DestroyGameObject(GetOwner());
	}
	else
		GamePlayStatic::DestroyGameObject(GetOwner());

	m_Pos = Transform()->GetRelativePos();
	m_Pos = m_Pos + m_CurVelocity * DT;
	Transform()->SetRelativePos(m_Pos);
}

void CBossSwordBeamScript::OnHit()
{
}

void CBossSwordBeamScript::BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
}

void CBossSwordBeamScript::Overlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
}

void CBossSwordBeamScript::EndOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
}

void CBossSwordBeamScript::InitSwordBeamInfo(CGameObject* _Shooter, CGameObject* _Target, Vec3 _Pos, float _InitSpeed,
											 float _MaxSpeed, float _LifeSpan, float _Damage, bool _Explode,
											 bool _Alive)
{
	m_Shooter = _Shooter;
	m_Target  = _Target;
	CProjectileScript::InitProjectileInfo(_Pos, _InitSpeed, _MaxSpeed, _LifeSpan, _Damage, _Explode, _Alive);

	// y축 좌표 통일시켜 검기가 평행하게 날아가도록 설정
	m_TargetPos	  = m_Target->Transform()->GetRelativePos();
	m_TargetPos.y = m_Shooter->Transform()->GetRelativePos().y;
	// m_TargetPos.y += 100.f;
	// m_TargetPos.x += 500.f;
}