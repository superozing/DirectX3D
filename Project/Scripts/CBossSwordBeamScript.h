﻿#pragma once
#include "CProjectileScript.h"

class CBossSwordBeamScript : public CProjectileScript
{
private:
	class CGameObject* m_Shooter; // 투사체를 발사하는 객체
	CGameObject*	   m_Target;  // 투사체가 향하는 객체

	Vec3 m_TargetPos; // 투사체가 향하는 객체 초기 위치

	Vec3 m_CurVelocity;
	Vec3 m_Gravity; // 중력

public:
	virtual void OnHit() override;
	void InitSwordBeamInfo(CGameObject* _Shooter, CGameObject* _Target, Vec3 _Pos, float _InitSpeed, float _MaxSpeed,
						   float _LifeSpan, float _Damage, bool _Explode, bool _Alive);

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider);
	virtual void Overlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider);
	virtual void EndOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider);

public:
	CLONE(CBossSwordBeamScript);
	CBossSwordBeamScript();
	~CBossSwordBeamScript();
};
