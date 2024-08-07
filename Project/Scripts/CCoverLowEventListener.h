#pragma once
#include "CEventListener.h"

class CCoverLowEventListener : public CEventListener
{
private:
	class CArona*			 m_pArona;
	class CTutorialGameMode* m_pGM;
	class CPlayerScript*	 m_pPlayerScript;
	class CMonster*			 m_pMonster1;
	class CMonster*			 m_pMonster2;

	void (CCoverLowEventListener::*tick_func)();

private:
	void Start();
	void ClearEvent();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CCoverLowEventListener);
	CCoverLowEventListener();
	~CCoverLowEventListener();
};
