#pragma once
#include <Engine\CScript.h>

class CBossSmokeWaveScript : public CScript
{
private:
	class CGameObject* m_Parent;

	Vec3 m_OriginScale;

	bool m_bSmokeWave;

public:
	void SetParent(CGameObject* _parent) { m_Parent = _parent; }

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CBossSmokeWaveScript);
	CBossSmokeWaveScript();
	~CBossSmokeWaveScript();
};
