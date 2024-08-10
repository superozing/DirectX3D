#pragma once
#include <Engine\CScript.h>

class CBossSwordTrailScript : public CScript
{
private:
	class CGameObject* m_Kaiten;

	float m_TrailOffset;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CBossSwordTrailScript);
	CBossSwordTrailScript();
	~CBossSwordTrailScript();
};
