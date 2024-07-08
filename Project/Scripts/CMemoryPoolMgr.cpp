#include "pch.h"
#include "CMemoryPoolMgr.h"
#include <Engine\CMemoryPool.h>

CMemoryPoolMgr::CMemoryPoolMgr()
	: CScript((UINT)SCRIPT_TYPE::MEMORYPOOLMGR)
{
}

CMemoryPoolMgr::~CMemoryPoolMgr()
{
}

void CMemoryPoolMgr::begin()
{
	list<CGameObject*> PoolList = CMemoryPool::GetInst()->GetList();

	for (auto iter = PoolList.begin(); iter != PoolList.end(); ++iter)
	{
		GamePlayStatic::SpawnGameObject((*iter), 30);
		GetOwner()->AddChild(*iter);
	}
}

void CMemoryPoolMgr::tick()
{
}
