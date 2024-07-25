#pragma once
#include <Engine/CScript.h>

class CBulletMarkSpawner : public CScript
{
private:
	list<pair<CGameObject*, float>> m_BulletDecalList;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	void SpawnBulletMarkDecal(Vec3 _PlayerFrontDir, Vec3 _HitPos, float _ActiveTime = 3.f);

public:
	CLONE(CBulletMarkSpawner);
	CBulletMarkSpawner();
	~CBulletMarkSpawner();
};
