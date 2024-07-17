#pragma once
#include <Engine\CScript.h>

struct Listner
{
	CScript* m_pLisnter;
	void	 (CScript::*Delegate)();
};

class CEventDetector : public CScript
{
private:
	vector<Listner> m_vecListners;

public:
	void PushCallBack(CScript* _script, void (CScript::*_delegate)());

public:
	virtual void tick() override;
	virtual void begin() override;

	virtual void BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider) override;
	virtual void Overlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider) {}
	virtual void EndOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider) {}

public:
	CLONE(CEventDetector);
	CEventDetector();
	~CEventDetector();

private:
	bool ComponentCheck();
};
