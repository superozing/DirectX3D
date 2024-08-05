#pragma once
#include <Engine/CScript.h>
#include "CWrapImage.h"

class CWallScript : public CScript
{
private:
public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CWallScript);
	CWallScript();
	~CWallScript();
};
