#pragma once
class CBulletHitParticleSpawner
{
private:
	list<pair<CGameObject*, float>> m_BulletShellList;

	class CMemoryPoolMgrScript* m_PoolMgr;

public:
	virtual void begin() ;
	virtual void tick() ;

public:
	void SpawnBulletHitParticle(const struct tRoRHitInfo& _HitInfo);

public:
	CLONE(CBulletHitParticleSpawner);
	CBulletHitParticleSpawner();
	~CBulletHitParticleSpawner();
};
