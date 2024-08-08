#pragma once
#include <Engine/CScript.h>

class CBossBulletShellSpawner : public CScript
{
private:
	list<pair<CGameObject*, float>> m_BulletShellList;

	class CMemoryPoolMgrScript* m_PoolMgr;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SpawnBossBulletShell(CGameObject* _pObj, float _ActiveTime = 2.f);

public:
	CLONE(CBossBulletShellSpawner);
	CBossBulletShellSpawner();
	~CBossBulletShellSpawner();
};
