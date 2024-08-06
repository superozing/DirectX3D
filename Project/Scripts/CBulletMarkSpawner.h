#pragma once
#include <Engine/CScript.h>

class CBulletMarkSpawner : public CScript
{
private:
	list<pair<CGameObject*, float>> m_BulletDecalList;
	int								m_iMaxDecalCount;

	Ptr<CTexture> m_NormalTargetTex;

	class CMemoryPoolMgrScript* m_PoolMgr;

public:
	virtual void begin() override;
	virtual void tick() override;


	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

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
