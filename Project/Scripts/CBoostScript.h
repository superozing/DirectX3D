#pragma once
#include <Engine\CScript.h>

class CBoostScript : public CScript
{
private:
	class CGameObject* m_Parent;

	Vec3			m_vDir; // 미사일 방향
	tParticleModule m_Module;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CBoostScript);
	CBoostScript();
	~CBoostScript();
};
