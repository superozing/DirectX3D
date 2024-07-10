#pragma once

class CMemoryPool
{
private:
	list<CGameObject*> m_listObjectPool;
	Ptr<CPrefab>	   m_PoolPrefab;

	int iPoolMaxCount;

public:
	void begin(wstring strPrefabrRelactivePath);

public:
	CGameObject* PopObject();
	void		 PushObject(CGameObject* _Object);

	int GetCurCount() { return m_listObjectPool.size(); }

public:
	CMemoryPool();
	~CMemoryPool();
};
