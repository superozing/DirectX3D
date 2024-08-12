#pragma once
#include "CAtlasImageUIScript.h"

class CBossOpeningTex : public CAtlasImageUIScript
{
private:
	int m_LVState;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CBossOpeningTex);
	CBossOpeningTex();
	~CBossOpeningTex();
};
