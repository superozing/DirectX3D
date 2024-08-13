#pragma once
#include "CAtlasImageUIScript.h"
class CTutOpeningTex : public CAtlasImageUIScript
{
private:
	int m_LVState;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CTutOpeningTex);
	CTutOpeningTex();
	~CTutOpeningTex();
};
