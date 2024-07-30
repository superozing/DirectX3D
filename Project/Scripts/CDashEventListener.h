#pragma once
#include "CEventListener.h"
class CDashEventListener : public CEventListener
{
public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CDashEventListener);
	CDashEventListener();
	~CDashEventListener();
};
