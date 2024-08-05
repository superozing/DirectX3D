#pragma once
#include "CEventListener.h"
class CCoverHIghTutorialEvent : public CEventListener
{
private:
	class CPlayerScript* m_pPlayerScript;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CCoverHIghTutorialEvent);
	CCoverHIghTutorialEvent();
	~CCoverHIghTutorialEvent();
};
