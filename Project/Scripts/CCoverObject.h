#pragma once
#include <Engine\CScript.h>

class CCoverObject : public CScript
{
public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CCoverObject);
	CCoverObject();
	~CCoverObject();
};
