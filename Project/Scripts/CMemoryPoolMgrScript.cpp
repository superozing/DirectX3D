﻿
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

		CGameObject* RegisterLayerobj = pPool->PopObject();

		// 레이어가 -1이 아닌 상태에서 자식으로 추가되는것에 대한 예외처리
		CMemoryPoolMgr::GetInst()->SavePrefabLayer(RegisterLayerobj);
		RegisterLayerobj->m_iLayerIdx = -1;
		pFilterObj->AddChild(RegisterLayerobj);

		for (int i = 0; pPool->GetCurCount() - 1; ++i)
		{
			CGameObject* pObj = pPool->PopObject();
			pObj->m_iLayerIdx = -1;
			pFilterObj->AddChild(pObj);
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

	// 이미 POOL이 존재하는 경우(POOL은 있지만 OBJ가 있는가 없는가에 대한 여부)
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

	// POOL이 없는 경우 EX에 필터를 만들고 PoolMgr를 통해 pop한다.
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

void CMemoryPoolMgrScript::PushObject(CGameObject* _Object)
{
	// EX OBJ의 Filter를 받아온다.
	vector<CGameObject*> vecObj = GetOwner()->GetChild();

	for (int i = 0; i < vecObj.size(); ++i)
	{
		string s = ToString(vecObj[i]->GetName());

		if (ToString(_Object->GetName()).find(s))
		{
			vecObj[i]->AddChild(_Object);
			return;
		}
	}

	CTaskMgr::GetInst()->SetMemoryPoolEvent(true);
}
