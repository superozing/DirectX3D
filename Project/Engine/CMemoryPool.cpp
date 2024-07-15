#include "pch.h"
#include "CMemoryPool.h"

#include "CPrefab.h"
#include "CGameObject.h"
#include "CTaskMgr.h"
#include "CMemoryPoolMgr.h"

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

	// 로딩 실패시
	if (pPrefab == nullptr)
	{
		wstring msg = ToWString(strPrefabrRelactivePath);
		msg += L" Init Fail";
		MessageBox(nullptr, msg.c_str(), L"Memory Pool Prefab Load Fail", MB_OK);
		return;
	}

	m_PoolPrefab = pPrefab;

	// layer를 -1로 바꿔야 한다. create pool을 해야 할 경우
	CGameObject* RegisterLayerObj = m_PoolPrefab->Instantiate();
	CMemoryPoolMgr::GetInst()->SavePrefabLayer(RegisterLayerObj);
	RegisterLayerObj->m_iLayerIdx = -1;

	m_listObjectPool.push_back(RegisterLayerObj);

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pObj = m_PoolPrefab->Instantiate();
		pObj->m_iLayerIdx = -1;
		m_listObjectPool.push_back(pObj);
	}
}

CGameObject* CMemoryPool::PopObject()
{
	CGameObject* pObj = nullptr;
	if (m_PoolPrefab == nullptr)
		return nullptr;

	// Pool이 비었으면 pool에서 만들어서 반환
	// Pool이 비어있지 않다면 list에서 하나 추출
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
	// 레벨에서 나가야 하기 때문에 -1로 만들어줘야 한다.
	if (_Object->GetLayerIdx() != -1)
	{
		if (_Object->GetParent() != nullptr)
			_Object->DisconnectWithParent();

		_Object->DisconnectWithLayer();
	}

	m_listObjectPool.push_back(_Object);

	CTaskMgr::GetInst()->SetMemoryPoolEvent(true);
}
