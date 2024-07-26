#pragma once
#include <Engine\CScript.h>

class CAfterImage : public CScript
{
public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CAfterImage);
	CAfterImage();
	~CAfterImage();
};
