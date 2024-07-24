#pragma once
#include <Engine\CScript.h>

class CBossShieldScript : public CScript
{
private:
	class CGameObject* m_Parent;

	float m_ShieldTime;

public:
	void SetParent(CGameObject* _parent) { m_Parent = _parent; }

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CBossShieldScript);
	CBossShieldScript();
	~CBossShieldScript();
};
