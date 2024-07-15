﻿#pragma once

#include "CManager.h"
#include "CTaskMgr.h"

class CMemoryPool;

class CMemoryPoolMgr : public CManager<CMemoryPoolMgr>
{
	SINGLE(CMemoryPoolMgr);

private:
	map<string, CMemoryPool*>	   m_mapMemoryPool;
	vector<std::pair<string, int>> m_vecPrefabLayer;

	CGameObject* m_pMemoryPoolEX;

public:
	CGameObject* PopObject(string _strMapKey);
	void		 PushObject(string _strMapKey, CGameObject* _Object);

	int GetMapCount() { return m_mapMemoryPool.size(); }
	int GetCurrentCount(string _strMapKey);

	void		 SetEX(CGameObject* pObj) { m_pMemoryPoolEX = pObj; }
	CGameObject* GetEX() { return m_pMemoryPoolEX; }

	map<string, CMemoryPool*> GetMapPool() { return m_mapMemoryPool; }

	void SavePrefabLayer(CGameObject* pObj);
	int	 GetPrefabLayer(CGameObject* pObj);

private:
	CMemoryPool* CreatePool(string _strMapKey);
	CMemoryPool* FindPool(string _strMapKey);

public:
	virtual void tick() override;
	virtual void init() override{};
	void		 Poolbegin(string strPrefabRelativePath);
	virtual void enter() override{};
};
