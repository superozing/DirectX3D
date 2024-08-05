#pragma once
#include <Engine/CScript.h>

// TODO : 몬스터 스크립트 생기면 상속 바꿔야 함
class CTutorialTarget : public CScript
{
public:
	bool m_bHit;

public:
	bool IsHit() { return m_bHit; }
	void Hit(bool _hit) { m_bHit = _hit; }

public:
	virtual void begin() override;
	virtual void tick() override;

	// virtual void BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider);

public:
	CLONE(CTutorialTarget);
	CTutorialTarget();
	~CTutorialTarget();
};
