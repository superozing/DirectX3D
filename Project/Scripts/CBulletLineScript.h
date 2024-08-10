#pragma once
#include <Engine\CScript.h>

class CBulletLineScript : public CScript
{
private:
	class CGameObject* m_Parent;
	CGameObject*	   m_Target;
	Vec3			   m_TargetPos;

	Vec3			m_vDir; // 미니건 방향
	tParticleModule m_Module;

	int m_CurFrame;
	int m_PrevFrame;

	bool m_bParticle;
	Vec3 m_offset;
	Vec3 m_offsetDir;

public:
	void SetParent(CGameObject* _Obj) { m_Parent = _Obj; }
	void SetTarget(CGameObject* _Obj) { m_Target = _Obj; }

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CBulletLineScript);
	CBulletLineScript();
	~CBulletLineScript();
};
