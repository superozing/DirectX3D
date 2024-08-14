#pragma once
#include "CAtlasImageUIScript.h"
class CTutVictoryTex : public CAtlasImageUIScript
{
private:
	class CTutorialGameMode* m_pGM;
	int						 m_LVState;

	bool m_EndingIn;
	bool m_EndingOut;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CTutVictoryTex);
	CTutVictoryTex();
	~CTutVictoryTex();
};
