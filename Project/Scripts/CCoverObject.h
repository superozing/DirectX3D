#pragma once
#include "CEventListener.h"

class CCoverObject : public CEventListener
{
public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CCoverObject);
	CCoverObject();
	~CCoverObject();
};
