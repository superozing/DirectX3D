#pragma once
#include "CEventListener.h"

#define SPAWNERCNT 5

class CShootingTutorialEvent : public CEventListener
{
private:
	class CPlayerScript*	 m_pPlayerScript;
	class CArona*			 m_pArona;
	class CGameObject*		 m_pWall;
	class CTutorialGameMode* m_pGM;

	vector<class CSpawnSpotScript*> m_vecTargets;
	vector<class CTutorialTarget*>	m_vecTargetScripts;

	bool m_vecClear[SPAWNERCNT];

	bool m_bFirstFrm;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CShootingTutorialEvent);
	CShootingTutorialEvent();
	~CShootingTutorialEvent();
};
