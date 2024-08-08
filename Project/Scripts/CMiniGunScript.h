#pragma once
#include <Engine\CScript.h>

class CMiniGunScript : public CScript
{
private:
	class CGameObject* m_Parent;

	Vec3			m_vDir; // 미니건 방향
	tParticleModule m_Module;

	bool m_bParticle;
	Vec3 m_offset;
	Vec3 m_offsetDir;

public:
	void SetParent(CGameObject* _Obj) { m_Parent = _Obj; }

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CMiniGunScript);
	CMiniGunScript();
	~CMiniGunScript();
};
