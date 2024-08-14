#pragma once
#include "CAtlasImageUIScript.h"
class CTutStartLightTex : public CAtlasImageUIScript
{
private:
	class CTutorialGameMode* m_pGM;
	int						 m_LVState;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CTutStartLightTex);
	CTutStartLightTex();
	~CTutStartLightTex();
};
