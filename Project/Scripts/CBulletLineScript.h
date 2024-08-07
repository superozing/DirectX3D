#pragma once
#include <Engine/CScript.h>
class CBulletLineScript : public CScript
{
private:
	// 아즈사
	CGameObject* m_Parent;

	// Bulletline이 날아갈 방향
	Vec3		 m_vDir;

	// 파티클 모듈(SetDirection 위해)
	tParticleModule m_Module;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CBulletLineScript);
	CBulletLineScript();
	~CBulletLineScript();
};
