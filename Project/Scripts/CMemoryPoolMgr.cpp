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
		GetOwner()->AddChild(*iter);
	}

	iPoolMaxCount = PoolList.size();
	iCurPopCount  = 0;

	CGameObject* pObj = dynamic_cast<CGameObject*>(GetOwner());
	CMemoryPool::GetInst()->RegisterPoolMgr(pObj);
}

void CMemoryPoolMgr::tick()
{
}

CGameObject* CMemoryPoolMgr::PopObject()
{
	list<CGameObject*> PoolList = CMemoryPool::GetInst()->GetList();

	if (PoolList.empty() && iCurPopCount == iPoolMaxCount)
	{
		CGameObject*   pObj		 = nullptr;
		Ptr<CMeshData> pMeshData = nullptr;
		pMeshData				 = CAssetMgr::GetInst()->LoadFBX(L"fbx\\TutorialTarget.fbx");

		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Target" + ToWString(std::to_string(iPoolMaxCount + 1)));

		++iCurPopCount;
		++iPoolMaxCount;

		return pObj;
	}
	else
	{
		CGameObject* pObj = nullptr;
		pObj			  = CMemoryPool::GetInst()->PopObject();
		pObj->DisconnectWithParent();
		++iCurPopCount;
		return pObj;
	}
}

#include <Engine\CLogMgr.h>
void CMemoryPoolMgr::PushObject(CGameObject* _Object)
{
	//_Object->DisconnectWithLayer();
	CMemoryPool::GetInst()->PushObject(_Object);
	--iCurPopCount;

	if (iCurPopCount == 0)
	{
		m_listObjectPool.sort([](CGameObject* a, CGameObject* b) { return a->GetName() < b->GetName(); });
	}

	GetOwner()->AddChild(_Object);

	vector<CGameObject*> count = GetOwner()->GetChild();

	CLogMgr::GetInst()->AddLog(Log_Level::INFO, string(std::to_string(count.size())));
}

void CMemoryPoolMgr::PopPool()
{
}

void CMemoryPoolMgr::PushPool()
{
}
