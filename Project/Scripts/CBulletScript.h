#pragma once
#include <Engine/CScript.h>
class CBulletScript : public CScript
{
private:
	Vec3 m_LinearVelocity;

	bool m_FirstFlag = false;

public:
	virtual void begin() override;
	virtual void tick() override;

	void SetLinearVelocity(Vec3 _LinearVelocity) { m_LinearVelocity = _LinearVelocity; }

public:
	CLONE(CBulletScript);
	CBulletScript();
	~CBulletScript();
};
