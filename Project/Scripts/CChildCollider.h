#pragma once
#include <Engine\CScript.h>

class CChildCollider : public CScript
{
private:
	class CGameObject* m_Parent;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider);
	virtual void Overlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider);
	virtual void EndOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider);

public:
	CLONE(CChildCollider);
	CChildCollider();
	~CChildCollider();
};
