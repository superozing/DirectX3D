#pragma once
#include "C:\Users\lee\source\repos\PortOrigin\External\Include\Engine\CScript.h"
class CCoverHigh : public CScript
{
private:
public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CCoverHigh);
	CCoverHigh();
	~CCoverHigh();
};