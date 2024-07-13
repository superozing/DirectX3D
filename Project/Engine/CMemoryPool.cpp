#include "pch.h"
#include "CMemoryPool.h"

#include "CPrefab.h"
#include "CGameObject.h"
#include "CTaskMgr.h"

CMemoryPool::CMemoryPool()
	: m_listObjectPool()
	, iPoolMaxCount(5)
	, m_PoolPrefab(nullptr)
{
}

CMemoryPool::~CMemoryPool()
{
	Delete_List(m_listObjectPool);
}

void CMemoryPool::begin(string strPrefabrRelactivePath)
{
	Ptr<CPrefab> pPrefab = CAssetMgr::GetInst()->Load<CPrefab>(strPrefabrRelactivePath);

	if (pPrefab == nullptr)
	{
		wstring msg = ToWString(strPrefabrRelactivePath);
		msg += L" Init Fail";
		MessageBox(nullptr, msg.c_str(), L"Memory Pool Prefab Load Fail", MB_OK);
		return;
	}

	m_PoolPrefab = pPrefab;

	for (int i = 0; i < 5; ++i)
	{
		m_listObjectPool.push_back(pPrefab->Instantiate());
	}
}

CGameObject* CMemoryPool::PopObject()
{
	CGameObject* pObj = nullptr;
	if (m_PoolPrefab == nullptr)
		return nullptr;

	if (m_listObjectPool.empty())
	{
		pObj = m_PoolPrefab->Instantiate();
		++iPoolMaxCount;
	}
	else
	{
		pObj = m_listObjectPool.front();
		m_listObjectPool.pop_front();
	}

	return pObj;
}

void CMemoryPool::PushObject(CGameObject* _Object)
{
	if (_Object->GetLayerIdx() != -1)
	{
		if (_Object->GetParent() != nullptr)
			_Object->DisconnectWithParent();

		_Object->DisconnectWithLayer();
	}

	m_listObjectPool.push_back(_Object);

	CTaskMgr::GetInst()->SetMemoryPoolEvent(true);

	// m_listObjectPool.sort([](CGameObject** a, CGameObject** b) { return (*a)->GetName() < (*b)->GetName(); });
}
