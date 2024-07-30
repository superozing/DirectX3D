#pragma once
#include "CEventListener.h"
class CShootingTutorialEvent : public CEventListener
{
public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CShootingTutorialEvent);
	CShootingTutorialEvent();
	~CShootingTutorialEvent();
};
