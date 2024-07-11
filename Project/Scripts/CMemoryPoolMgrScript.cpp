
#include "pch.h"
#include "CMemoryPoolMgrScript.h"

#include <Engine\CMemoryPoolMgr.h>
#include <Engine\CTaskMgr.h>

CMemoryPoolMgrScript::CMemoryPoolMgrScript()
	: CScript((UINT)SCRIPT_TYPE::MEMORYPOOLMGRSCRIPT)
{
}

CMemoryPoolMgrScript::~CMemoryPoolMgrScript()
{
}

void CMemoryPoolMgrScript::begin()
{
	CMemoryPoolMgr::GetInst()->begin();

	int							   iCurruentCount = CMemoryPoolMgr::GetInst()->GetMapCount();
	vector<std::pair<string, int>> PoolInfo		  = CMemoryPoolMgr::GetInst()->GetPoolKeys();

	for (int i = 0; i < iCurruentCount; ++i)
	{
		GetOwner()->AddChild(CMemoryPoolMgr::GetInst()->PopObject(PoolInfo[i].first));
	}
}

void CMemoryPoolMgrScript::tick()
{
}

CGameObject* CMemoryPoolMgrScript::PopObject(string _strMapKey)
{
	CGameObject*		 pObj	  = nullptr;
	vector<CGameObject*> vecChild = GetOwner()->GetChild();

	CTaskMgr::GetInst()->SetMemoryPoolEvent(true);

	if (vecChild.size() == 0)
	{
		pObj = CMemoryPoolMgr::GetInst()->PopObject(_strMapKey);
	}
	else
	{
		pObj = vecChild.back();
		pObj->DisconnectWithParent();
	}
	return pObj;
}

void CMemoryPoolMgrScript::PushObject(CGameObject* _Object)
{
	// 자식에 대한 자식 예정
	GetOwner()->AddChild(_Object);
	CTaskMgr::GetInst()->SetMemoryPoolEvent(true);
}
