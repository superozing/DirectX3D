
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
	CMemoryPoolMgr::GetInst()->begin(PREFTutorialTarget2);

	int iCurruentCount = CMemoryPoolMgr::GetInst()->GetCurrentCount();

	for (int i = 0; i < iCurruentCount; ++i)
	{
		GetOwner()->AddChild(CMemoryPoolMgr::GetInst()->PopObject());
	}
}

void CMemoryPoolMgrScript::tick()
{
}

CGameObject* CMemoryPoolMgrScript::PopObject()
{
	CGameObject*		 pObj	  = nullptr;
	vector<CGameObject*> vecChild = GetOwner()->GetChild();

	CTaskMgr::GetInst()->SetMemoryPoolEvent(true);

	if (vecChild.size() == 0)
	{
		pObj = CMemoryPoolMgr::GetInst()->PopObject();
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
