#pragma once
class CBulletMarkSpawner
{
private:
	list<pair<CGameObject*, float>> m_BulletDecalList;
	int								m_iMaxDecalCount;

	Ptr<CTexture> m_NormalTargetTex;

	class CMemoryPoolMgrScript* m_PoolMgr;

public:
	virtual void begin();
	virtual void tick();

public:
	void SpawnBulletMarkDecal(const struct tRoRHitInfo& _HitInfo, CGameObject* _pPlayer, float _ActiveTime = 3.f);

private:
	Vec3 GetCenterNormal();
	Vec3 TransformNormalToWorld(const Vec3& normal);

public:
	CLONE(CBulletMarkSpawner);
	CBulletMarkSpawner();
	~CBulletMarkSpawner();
};
