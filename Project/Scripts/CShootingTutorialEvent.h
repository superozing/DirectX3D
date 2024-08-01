#pragma once
#include "CEventListener.h"
class CShootingTutorialEvent : public CEventListener
{
private:
	class CPlayerScript*	 m_pPlayerScript;
	class CArona*			 m_pArona;
	class CGameObject*		 m_pWall;
	class CTutorialGameMode* m_pGM;

	vector<CGameObject*> m_vecTargets;

	bool m_bFirstFrm;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CShootingTutorialEvent);
	CShootingTutorialEvent();
	~CShootingTutorialEvent();
};
