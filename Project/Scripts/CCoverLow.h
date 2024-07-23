#pragma once
#include <Engine/CScript.h>
class CCoverLow : public CScript
{
private:
public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CCoverLow);
	CCoverLow();
	~CCoverLow();
};
