#pragma once

#include <Engine/CScript.h>
class CTitle : public CScript
{
public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CTitle);
	CTitle();
	~CTitle();
};
