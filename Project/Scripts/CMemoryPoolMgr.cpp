#include "pch.h"
#include "CMemoryPoolMgr.h"
#include <Engine\CMemoryPool.h>

CMemoryPoolMgr::CMemoryPoolMgr()
	: CScript((UINT)SCRIPT_TYPE::MEMORYPOOLMGR)
{
}

CMemoryPoolMgr::~CMemoryPoolMgr()
{
	vector<CGameObject*> vecChild = GetOwner()->GetChild();

	for (int i = 0; i < vecChild.size(); ++i)
	{
		vecChild[i]->DisconnectWithParent();
	}
}

void CMemoryPoolMgr::begin()
{

	for (auto iter = m_listObjectPool.begin(); iter != m_listObjectPool.end(); ++iter)
	{
		GetOwner()->AddChild(*iter);
	}

	iPoolMaxCount = m_listObjectPool.size();
	iCurPopCount  = 0;

	CGameObject* pObj = dynamic_cast<CGameObject*>(GetOwner());
	CMemoryPool::GetInst()->RegisterPoolMgr(pObj);
}

void CMemoryPoolMgr::tick()
{
}

CGameObject* CMemoryPoolMgr::PopObject()
{
	CGameObject* pObj = nullptr;
	pObj			  = CMemoryPool::GetInst()->PopObject();
	return pObj;
}

void CMemoryPoolMgr::PushObject(CGameObject* _Object)
{
	CMemoryPool::GetInst()->PushObject(_Object);
	GetOwner()->AddChild(_Object);
}

void CMemoryPoolMgr::PopPool()
{
}

void CMemoryPoolMgr::PushPool()
{
}
