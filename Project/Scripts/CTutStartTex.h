#pragma once
#include "CAtlasImageUIScript.h"
class CTutStartTex : public CAtlasImageUIScript
{
private:
	class CGameObject* m_pGM;
	int				   m_LVState;

	bool m_StartIn;
	bool m_StartDelay;
	bool m_StartOut;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CTutStartTex);
	CTutStartTex();
	~CTutStartTex();
};
