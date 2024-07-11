#pragma once

#include "CManager.h"

class CMemoryPool;

class CMemoryPoolMgr : public CManager<CMemoryPoolMgr>
{
	SINGLE(CMemoryPoolMgr);

private:
	map<string, CMemoryPool*> m_mapMemoryPool;

	CGameObject* m_pMemoryPoolEX;

public:
	CGameObject* PopObject(string _strMapKey);
	void		 PushObject(string _strMapKey, CGameObject* _Object);

	int GetMapCount() { return m_mapMemoryPool.size(); }
	int GetCurrentCount(string _strMapKey);

	void		 SetEX(CGameObject* pObj) { m_pMemoryPoolEX = pObj; }
	CGameObject* GetEX() { return m_pMemoryPoolEX; }

	vector<std::pair<string, int>> GetPoolKeys();
	void						   SavePool();

private:
	CMemoryPool* FindPool(string _strMapKey);

public:
	virtual void tick() override;
	virtual void init() override{};
	void		 begin();
	virtual void enter() override{};
};
