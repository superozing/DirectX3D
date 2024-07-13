
#include "pch.h"
#include "CMemoryPoolMgrScript.h"

#include <Engine\CMemoryPool.h>
#include <Engine\CMemoryPoolMgr.h>
#include <Engine\CTaskMgr.h>
#include <Engine\components.h>

CMemoryPoolMgrScript::CMemoryPoolMgrScript()
	: CScript((UINT)SCRIPT_TYPE::MEMORYPOOLMGRSCRIPT)
{
}

CMemoryPoolMgrScript::~CMemoryPoolMgrScript()
{
}

void CMemoryPoolMgrScript::begin()
{
	map<string, CMemoryPool*> mapMemory = CMemoryPoolMgr::GetInst()->GetMapPool();

	for (auto iter = mapMemory.begin(); iter != mapMemory.end(); ++iter)
	{
		CMemoryPool* pPool = iter->second;

		CGameObject* pFilterObj = new CGameObject;
		pFilterObj->AddComponent(new CTransform);
		pFilterObj->SetName(iter->first);

		GetOwner()->AddChild(pFilterObj);

		for (int i = 0; pPool->GetCurCount(); ++i)
		{
			pFilterObj->AddChild(pPool->PopObject());
		}
	}

	CTaskMgr::GetInst()->SetMemoryPoolEvent(true);
}

void CMemoryPoolMgrScript::tick()
{
}

CGameObject* CMemoryPoolMgrScript::PopObject(string _strMapKey)
{
	CGameObject*		 pObj	  = nullptr;
	vector<CGameObject*> vecChild = GetOwner()->GetChild();

	CTaskMgr::GetInst()->SetMemoryPoolEvent(true);
	for (int i = 0; i < vecChild.size(); ++i)
	{
		if (ToString(vecChild[i]->GetName()) == _strMapKey && vecChild[i]->GetChild().size() == 0)
		{
			pObj = CMemoryPoolMgr::GetInst()->PopObject(_strMapKey);
		}
		else if (ToString(vecChild[i]->GetName()) == _strMapKey && vecChild[i]->GetChild().size() > 0)
		{
			pObj = vecChild[i]->GetChild().back();
			pObj->DisconnectWithParent();
		}
	}

	if (pObj == nullptr)
	{
		CGameObject* pFilterObj = new CGameObject;
		pFilterObj->AddComponent(new CTransform);
		pFilterObj->SetName(_strMapKey);

		GetOwner()->AddChild(pFilterObj);

		pObj = CMemoryPoolMgr::GetInst()->PopObject(_strMapKey);
	}

	return pObj;
}

#include <Engine\CLogMgr.h>
void CMemoryPoolMgrScript::PushObject(CGameObject* _Object)
{
	vector<CGameObject*> vecObj = GetOwner()->GetChild();

	for (int i = 0; i < vecObj.size(); ++i)
	{
		string s = ToString(vecObj[i]->GetName());

		if (ToString(_Object->GetName()).find(s))
		{
			vecObj[i]->AddChild(_Object);
		}
	}

	CTaskMgr::GetInst()->SetMemoryPoolEvent(true);
}
