#include "pch.h"
#include "CMemoryPoolMgr.h"
#include "CMemoryPool.h"

#include "CGameObject.h"
#include "CTaskMgr.h"

#include "CTransform.h"

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
	// pool 있으면 넘아가고, 없으면 만들어서 begin 시킨다.

	CMemoryPool* pFindPool = FindPool(strPrefabRelativePath);

	if (pFindPool == nullptr)
	{
		CMemoryPool* pPool = CreatePool(strPrefabRelativePath);
	}
	else
		return;
}

CGameObject* CMemoryPoolMgr::PopObject(string _strMapKey)
{
	CMemoryPool* pPool = FindPool(_strMapKey);

	// Pool이 없으면 동적으로 만든다.
	if (pPool == nullptr)
	{
		pPool = CreatePool(_strMapKey);
	}

	return pPool->PopObject();
}

#include "CPhysX.h"
void CMemoryPoolMgr::PushObject(string _strMapKey, CGameObject* _Object)
{
	// Pool 역할을 하는 EX를 먼저 가져온다. 필터가 존재하면 넣는다.
	vector<CGameObject*> vecObj = m_pMemoryPoolEX->GetChild();

	for (int i = 0; i < vecObj.size(); ++i)
	{
		string strFind = ToString(vecObj[i]->GetName());

		if (strFind.find(_strMapKey) != string::npos)
		{
			if (nullptr != _Object->PhysX())
			{
				_Object->PhysX()->releaseActor();
			}
			vecObj[i]->AddChild(_Object);
			CTaskMgr::GetInst()->SetMemoryPoolEvent(true);
			return;
		}
	}

	CMemoryPool* pPool = FindPool(_strMapKey);
	pPool->PushObject(_Object);
}

string CMemoryPoolMgr::GetBaseName(const string& strFullname)
{
	size_t startOfCopy = strFullname.find("_copy");

	if (startOfCopy == string::npos)
		return strFullname;
	else
	{
		return strFullname.substr(0, startOfCopy);
	}
}

void CMemoryPoolMgr::PushObject(CGameObject* _Object)
{
	// Pool 역할을 하는 EX를 먼저 가져온다. 필터가 존재하면 넣는다.
	vector<CGameObject*> vecObj = m_pMemoryPoolEX->GetChild();

	string strObjName = ToString(_Object->GetName());
	strObjName		  = GetBaseName(strObjName);

	for (int i = 0; i < vecObj.size(); ++i)
	{
		string strFind = ToString(vecObj[i]->GetName());

		if (strFind.find(strObjName) != string::npos)
		{
			if (nullptr != _Object->PhysX())
			{
				_Object->PhysX()->releaseActor();
			}
			vecObj[i]->AddChild(_Object);
			CTaskMgr::GetInst()->SetMemoryPoolEvent(true);
			return;
		}
	}
}

int CMemoryPoolMgr::GetCurrentCount(string _strMapKey)
{
	CMemoryPool* pPool = FindPool(_strMapKey);
	return pPool->GetCurCount();
}

void CMemoryPoolMgr::SavePrefabLayer(CGameObject* pObj)
{
	string strObjname = ToString(pObj->GetName());
	bool   bSetLayer  = false;

	for (int i = 0; i < m_vecPrefabLayer.size(); ++i)
	{
		if (strObjname.find(m_vecPrefabLayer[i].first) != string::npos)
		{
			bSetLayer = true;
			break;
		}
	}

	if (bSetLayer == true)
		return;
	else
		m_vecPrefabLayer.push_back(make_pair(strObjname, pObj->GetLayerIdx()));
}

int CMemoryPoolMgr::GetPrefabLayer(CGameObject* pObj)
{
	string strObjname = ToString(pObj->GetName());

	for (int i = 0; i < m_vecPrefabLayer.size(); ++i)
	{
		if (strObjname.find(m_vecPrefabLayer[i].first) != string::npos)
		{
			return m_vecPrefabLayer[i].second;
		}
	}
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
