#pragma once
#include "CEventListener.h"
class CTutorialEndingEvent : public CEventListener
{
private:
	class CArona*			 m_pArona;
	class CPlayerScript*	 m_pPlayerScript;
	class CTutorialGameMode* m_pGM;

	void (CTutorialEndingEvent::*tick_func)();

private:
	void Start();
	void ClearEvent();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CTutorialEndingEvent);
	CTutorialEndingEvent();
	~CTutorialEndingEvent();
};
