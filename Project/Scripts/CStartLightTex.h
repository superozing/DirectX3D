#pragma once
#include "CAtlasImageUIScript.h"

class CStartLightTex : public CAtlasImageUIScript
{
private:
	class CGameObject* m_BossLV;
	int				   m_LVState;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CStartLightTex);
	CStartLightTex();
	~CStartLightTex();
};
