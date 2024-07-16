#pragma once
#include <Engine\CScript.h>

class CPlayerDamagedScript : public CScript
{
private:
	class CGameObject* m_Vignette;

	bool  m_IsDamaged;
	float m_VignetteAlpha;
	float m_Duration;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CPlayerDamagedScript);
	CPlayerDamagedScript();
	~CPlayerDamagedScript();
};
