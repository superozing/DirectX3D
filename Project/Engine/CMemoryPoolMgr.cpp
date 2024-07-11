#include "pch.h"
#include "CMemoryPoolMgr.h"
#include "CMemoryPool.h"

CMemoryPoolMgr::CMemoryPoolMgr()
	: m_pMemoryPoolEX(nullptr)
{
	m_pPool = new CMemoryPool;

	CMemoryPool* MonsterPool = new CMemoryPool;
	m_mapMemoryPool.insert(make_pair("Monster", MonsterPool));
}

CMemoryPoolMgr::~CMemoryPoolMgr()
{
	if (m_pPool != nullptr)
		delete m_pPool;

	Delete_Map<string, CMemoryPool*>(m_mapMemoryPool);
}

void CMemoryPoolMgr::begin(wstring strPrefabRelativePath)
{
	m_pPool->begin(strPrefabRelativePath);
}

CGameObject* CMemoryPoolMgr::PopObject()
{
	return m_pPool->PopObject();
}

void CMemoryPoolMgr::PushObject(CGameObject* _Object)
{
	m_pPool->PushObject(_Object);
}

int CMemoryPoolMgr::GetCurrentCount()
{
	return m_pPool->GetCurCount();
}

vector<std::pair<string, int>> CMemoryPoolMgr::GetPoolKeys()
{
	vector<std::pair<string, int>> res;

	for (auto iter = m_mapMemoryPool.begin(); iter != m_mapMemoryPool.end(); ++iter)
	{
		std::pair<string, int> p;
		p.first	 = (iter->first);
		p.second = iter->second->GetMaxCount();

		res.push_back(p);
	}

	return res;
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
