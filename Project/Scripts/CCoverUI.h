#pragma once
#include "CEventListener.h"
class CCoverUI : public CEventListener
{
private:
	class CPlayerScript*	  m_pPlayerScript;
	vector<class CCoverArea*> m_vecUI;

public:
	void DrawUI(bool _draw);

public:
	virtual void begin() override;
	virtual void tick() override;
	virtual void BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider) override;
	virtual void EndOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider) override;

public:
	CLONE(CCoverUI);
	CCoverUI();
	~CCoverUI();
};
