#pragma once
#include <Engine\CScript.h>

enum class SHIELD_TYPE
{
	InnerShield,
	OutsideShield,
	END,
};

class CBossShieldScript : public CScript
{
private:
	class CGameObject* m_Parent;
	CGameObject*	   m_Triangle;

	float m_ShieldTime;

	SHIELD_TYPE m_ShieldType;

public:
	void SetParent(CGameObject* _parent) { m_Parent = _parent; }
	void SetShieldType(SHIELD_TYPE _type) { m_ShieldType = _type; }

	void CheckInnerShield();
	void CheckOutsideShield();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CBossShieldScript);
	CBossShieldScript();
	~CBossShieldScript();
};
