#pragma once
#include "CAtlasImageUIScript.h"

class CStartTex : public CAtlasImageUIScript
{
private:
	class CGameObject* m_BossLV;
	int				   m_LVState;

	bool m_StartIn;
	bool m_StartDelay;
	bool m_StartOut;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CStartTex);
	CStartTex();
	~CStartTex();
};
