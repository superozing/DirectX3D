#pragma once
#include "CEventListener.h"
class CCoverJumpTutorialEvent : public CEventListener
{
private:
	class CArona*			 m_pArona;
	class CPlayerScript*	 m_pPlayerScript;
	class CTutorialGameMode* m_pGM;

	void (CCoverJumpTutorialEvent::*tick_func)();

private:
	void Start();
	void EventClear();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CCoverJumpTutorialEvent);
	CCoverJumpTutorialEvent();
	~CCoverJumpTutorialEvent();
};
