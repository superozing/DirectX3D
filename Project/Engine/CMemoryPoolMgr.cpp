﻿#include "pch.h"
#include "CMemoryPoolMgr.h"
#include "CMemoryPool.h"

#define TagPoolCount "[CurrentPoolCount]"
#define TagMemoryPoolPrefab "[Pool Prefab]"
#define TagMemoryPoolCount "[PoolCount]"

CMemoryPoolMgr::CMemoryPoolMgr()
	: m_pMemoryPoolEX(nullptr)
{
}

CMemoryPoolMgr::~CMemoryPoolMgr()
{
	Delete_Map<string, CMemoryPool*>(m_mapMemoryPool);
}

void CMemoryPoolMgr::Poolbegin(string strPrefabRelativePath)
{
	CMemoryPool* pFindPool = FindPool(strPrefabRelativePath);

	if (pFindPool == nullptr)
	{
		CMemoryPool* pPool = CreatePool(strPrefabRelativePath);
	}
	else
		pFindPool->begin(strPrefabRelativePath);
}

CGameObject* CMemoryPoolMgr::PopObject(string _strMapKey)
{
	CMemoryPool* pPool = FindPool(_strMapKey);
	return pPool->PopObject();
}

void CMemoryPoolMgr::PushObject(string _strMapKey, CGameObject* _Object)
{
	CMemoryPool* pPool = FindPool(_strMapKey);
	pPool->PushObject(_Object);
}

int CMemoryPoolMgr::GetCurrentCount(string _strMapKey)
{
	CMemoryPool* pPool = FindPool(_strMapKey);
	return pPool->GetCurCount();
}

CMemoryPool* CMemoryPoolMgr::CreatePool(string _strMapKey)
{
	CMemoryPool* pPool = new CMemoryPool;
	pPool->begin(_strMapKey);
	m_mapMemoryPool.insert(make_pair(_strMapKey, pPool));

	return pPool;
}

CMemoryPool* CMemoryPoolMgr::FindPool(string _strMapKey)
{
	map<string, CMemoryPool*>::iterator iter = m_mapMemoryPool.find(_strMapKey);

	if (iter == m_mapMemoryPool.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CMemoryPoolMgr::tick()
{
}
