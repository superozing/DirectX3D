#pragma once

class CMemoryPool
{
private:
	list<CGameObject*> m_listObjectPool; // 메모리풀
	Ptr<CPrefab>	   m_PoolPrefab;	 // 풀에 할당된 prefab

	int iPoolMaxCount;

public:
	void begin(string strPrefabrRelactivePath);

public:
	CGameObject* PopObject();
	void		 PushObject(CGameObject* _Object);

	int GetCurCount() { return m_listObjectPool.size(); }
	int GetMaxCount() { return iPoolMaxCount; }

	void SetPool(int _Count);

public:
	CMemoryPool();
	~CMemoryPool();
};
