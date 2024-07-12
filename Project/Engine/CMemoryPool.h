#pragma once

class CMemoryPool
{
private:
	list<CGameObject*> m_listObjectPool;
	Ptr<CPrefab>	   m_PoolPrefab;

	int iPoolMaxCount;

public:
	void begin(string strPrefabrRelactivePath);

public:
	CGameObject* PopObject();
	void		 PushObject(CGameObject* _Object);

	int GetCurCount() { return m_listObjectPool.size(); }
	int GetMaxCount() { return iPoolMaxCount; }

public:
	CMemoryPool();
	~CMemoryPool();
};
