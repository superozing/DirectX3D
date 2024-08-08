#pragma once

struct tDamageFontNode
{
	CGameObject*	DamageFont;
	float			ActiveTime;
	float			MaxTime;
};

class CDamageFontSpawner
{
private:
	list<tDamageFontNode>	m_DamageFontList;
	int						m_iMaxDamageFontCount;

	Ptr<CTexture> m_NormalTargetTex;

	class CMemoryPoolMgrScript* m_PoolMgr;

public:
	virtual void begin() ;
	virtual void tick() ;

public:
	void SpawnDamageFont(const Vec3& _HitPos, int Damage, float _ActiveTime = 0.5f);

public:
	CLONE(CDamageFontSpawner);
	CDamageFontSpawner();
	~CDamageFontSpawner();
};
