#pragma once

#include <Engine\CScript.h>

class CTurretBulletLine : public CScript
{
private:
	class CGameObject* m_Parent;
	Vec3			   m_TargetPos;

	Vec3			m_vDir; // 미니건 방향
	tParticleModule m_Module;

	int m_CurFrame;
	int m_PrevFrame;

	bool m_bParticle;
	Vec3 m_offset;
	Vec3 m_offsetDir;

	float fParticleTimer;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CTurretBulletLine);
	CTurretBulletLine();
	~CTurretBulletLine();
};
