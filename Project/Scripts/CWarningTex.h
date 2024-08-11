#pragma once
#include "CAtlasImageUIScript.h"

class CWarningTex : public CAtlasImageUIScript
{
private:
	class CGameObject* m_BossLV;
	int				   m_LVState;

	bool m_WarningIn;
	bool m_WarningOut;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CWarningTex);
	CWarningTex();
	~CWarningTex();
};
