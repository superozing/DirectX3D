#pragma once

struct tBulletWarheadNode
{
	CGameObject* BulletWarhead;
	Vec3		 Dir;
	float		 ActiveTime;
};

class CBulletWarheadSpawner
{
private:
	list<tBulletWarheadNode> m_BulletWarheadList;

	class CMemoryPoolMgrScript* m_PoolMgr;

public:
	virtual void begin();
	virtual void tick();


public:
	void SpawnBulletWarhead(CGameObject* _pPlayer);

public:
	CLONE(CBulletWarheadSpawner);
	CBulletWarheadSpawner();
	~CBulletWarheadSpawner();
};
