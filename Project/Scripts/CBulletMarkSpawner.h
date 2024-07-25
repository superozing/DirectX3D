#pragma once
#include <Engine/CScript.h>

class CBulletMarkSpawner : public CScript
{
private:
	list<CGameObject*> m_BulletDecalList;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

private:
	void SetParentPanelUI();

public:
	void SpawnBulletMarkDecal();

public:
	CLONE(CBulletMarkSpawner);
	CBulletMarkSpawner();
	~CBulletMarkSpawner();
};
