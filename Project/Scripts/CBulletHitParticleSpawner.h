#pragma once
#include <Engine/CScript.h>
class CBulletHitParticleSpawner : public CScript
{
private:
	list<pair<CGameObject*, float>> m_BulletShellList;

	class CMemoryPoolMgrScript* m_PoolMgr;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	void SpawnBulletHitParticle(const struct tRoRHitInfo& _HitInfo);

public:
	CLONE(CBulletHitParticleSpawner);
	CBulletHitParticleSpawner();
	~CBulletHitParticleSpawner();
};
