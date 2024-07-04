#pragma once
#include <Engine/CScript.h>
class CHalo : public CScript
{
public:
	virtual void tick() override;
	virtual void begin() override;

public:
	CLONE(CHalo);
	CHalo();
	~CHalo();
};
