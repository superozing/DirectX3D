#pragma once
#include "CEventListener.h"
class CCoverHIghTutorialEvent : public CEventListener
{
private:
	class CPlayerScript*	 m_pPlayerScript;
	class CArona*			 m_pArona;
	class CTutorialGameMode* m_pGM;
	class CMonster*			 m_pMonster1;
	class CMonster*			 m_pMonster2;

	bool m_bStart;
	bool m_bCoverClear;
	bool m_bCombatClear;

	void (CCoverHIghTutorialEvent::*tick_func)();

private:
	void Start();
	void Cover();
	void Combat();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CCoverHIghTutorialEvent);
	CCoverHIghTutorialEvent();
	~CCoverHIghTutorialEvent();
};
