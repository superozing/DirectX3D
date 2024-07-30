#pragma once
#include "CEventListener.h"
class CWelcomeTutorialEvent : public CEventListener
{
public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CWelcomeTutorialEvent);
	CWelcomeTutorialEvent();
	~CWelcomeTutorialEvent();
};
