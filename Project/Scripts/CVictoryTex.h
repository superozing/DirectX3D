#pragma once
#include "CAtlasImageUIScript.h"

class CVictoryTex : public CAtlasImageUIScript
{
private:
	class CGameObject* m_BossLV;
	int				   m_LVState;

	bool m_EndingIn;
	bool m_EndingOut;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CVictoryTex);
	CVictoryTex();
	~CVictoryTex();
};
