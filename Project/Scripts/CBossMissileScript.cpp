#include "pch.h"
#include "CBossMissileScript.h"

#include <Engine\CRandomMgr.h>
#include "CPlayerScript.h"
#include "CBossScript.h"
#include "CParticleSpawnScript.h"

CBossMissileScript::CBossMissileScript()
	: CProjectileScript((UINT)SCRIPT_TYPE::BOSSMISSILESCRIPT)
	, m_Shooter(nullptr)
	, m_Target(nullptr)
	, m_TargetPos(0.f, 0.f, 0.f)
	, m_Gravity(0.f, -981.f, 0.f)
	, m_ForceAccTime(0.f)
	, m_ForceDuration(0.6f)
	, m_CurForce{}
{
}

CBossMissileScript::~CBossMissileScript()
{
}

void CBossMissileScript::begin()
{
	CProjectileScript::begin();

	Transform()->SetRelativePos(m_Pos);

	Vec3  VelDir = m_TargetPos - m_Pos;
	float Offset = VelDir.Length();
	VelDir.y	 = 0.f;
	VelDir.Normalize();

	float speed	  = m_InitialSpeed * (Offset / 1100.f);
	m_CurVelocity = VelDir * speed;
	Transform()->SetDir(m_CurVelocity);
}

void CBossMissileScript::tick()
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

	m_ForceAccTime += DT;

	// 처음엔 발사된 방향 그대로 직진
	if (m_ForceDuration > 0 && m_ForceAccTime < 0.1f)
	{
	}
	// 위쪽 방향으로 가속 & 수평 움직임도 랜덤하게 부여
	else if (0.1f <= m_ForceAccTime && m_ForceAccTime < m_ForceDuration)
	{
		float			   x, y, z;
		static const float minval = 1600.f;
		static const float maxval = 1800.f;
		x						  = CRandomMgr::GetInst()->GetRandomFloat(-maxval, maxval);
		y						  = CRandomMgr::GetInst()->GetRandomFloat(minval, maxval);
		z						  = CRandomMgr::GetInst()->GetRandomFloat(-maxval, maxval);
		m_CurForce				  = Vec3(x, y, z);
		m_CurVelocity += m_CurForce * DT;
	}
	// 아래쪽 방향으로 가속
	else
	{
		Vec3 toTarget = m_TargetPos - m_Pos;
		toTarget.Normalize();
		m_CurVelocity += ((toTarget * m_MaxSpeed + m_Gravity) * 4.f) * DT;
	}

	Transform()->SetDir(m_CurVelocity);
	// m_CurVelocity = Vec3(0.f, 0.f, 100.f);
	m_Pos = Transform()->GetRelativePos();
	m_Pos += m_CurVelocity * DT;

	Transform()->SetRelativePos(m_Pos);
	// Transform()->SetRelativePos(Vec3(-750.f, 200.f, 7540.f));
}

void CBossMissileScript::OnHit()
{
	m_Target->GetScript<CPlayerScript>()->Hit(m_Damage);
}

void CBossMissileScript::InitBossMissileInfo(CGameObject* _Shooter, CGameObject* _Target, Vec3 _Pos, float _InitSpeed,
											 float _MaxSpeed, float _LifeSpan, float _Damage, bool _Explode,
											 bool _Alive)
{
	m_Shooter = _Shooter;
	m_Target  = _Target;
	CProjectileScript::InitProjectileInfo(_Pos, _InitSpeed, _MaxSpeed, _LifeSpan, _Damage, _Explode, _Alive);

	m_TargetPos = m_Target->Transform()->GetRelativePos();

	// 타겟 위치 기준 오차 범위 생성
	float		x, z;
	const float MaxVal = 600.f;
	x				   = CRandomMgr::GetInst()->GetRandomFloat(-MaxVal, MaxVal);
	z				   = CRandomMgr::GetInst()->GetRandomFloat(-MaxVal, MaxVal);
	m_TargetPos.x += x;
	m_TargetPos.z += z;
}

void CBossMissileScript::BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
	// 미사일 충돌 시 폭발 파티클 스폰
	CGameObject* pObj	  = CAssetMgr::GetInst()->Load<CPrefab>(PREFp_Explode)->Instantiate();
	int			 layeridx = pObj->GetLayerIdx();
	GamePlayStatic::SpawnGameObject(pObj, layeridx);
	pObj->GetScript<CParticleSpawnScript>()->SetParticleInfo(Transform()->GetRelativePos(), 0.5f);

	if (IsRedZone())
		OnHit();

	m_IsAlive = false;
}

void CBossMissileScript::Overlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
}

void CBossMissileScript::EndOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
}

bool CBossMissileScript::IsRedZone()
{
	Vec3 vPos		= Transform()->GetRelativePos();
	Vec3 vTargetPos = m_Target->Transform()->GetRelativePos();

	float dist = (vTargetPos - vPos).Length();

	return dist >= 200.f ? false : true;
}
