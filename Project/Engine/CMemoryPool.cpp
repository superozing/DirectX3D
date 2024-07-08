#include "pch.h"
#include "CMemoryPool.h"

#include "CGameObject.h"

CMemoryPool::CMemoryPool()
	: m_listObjectPool()
	, iPoolMaxCount(5)
	, iCurPopCount(0)
{
}

CMemoryPool::~CMemoryPool()
{
}

void CMemoryPool::init()
{
	for (int i = 0; i < 5; ++i)
	{
		CGameObject*   pObj		 = nullptr;
		Ptr<CMeshData> pMeshData = nullptr;
		pMeshData				 = CAssetMgr::GetInst()->LoadFBX(L"fbx\\TutorialTarget.fbx");

		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Target" + ToWString(std::to_string(i)));
		m_listObjectPool.push_back(pObj);
	}
}

void CMemoryPool::tick()
{
}

CGameObject* CMemoryPool::PopObject()
{
	if (m_listObjectPool.empty() && iCurPopCount == iPoolMaxCount)
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
		CGameObject* obj = m_listObjectPool.front();
		m_listObjectPool.pop_front();
		++iCurPopCount;
		return obj;
	}
}

void CMemoryPool::PushObject(CGameObject* _Object)
{
	_Object->DisconnectWithLayer();
	m_listObjectPool.push_back(_Object);
	--iCurPopCount;

	if (iCurPopCount == 0)
	{
		m_listObjectPool.sort([](CGameObject* a, CGameObject* b) { return a->GetName() < b->GetName(); });

		string s;

		for (auto iter = m_listObjectPool.begin(); iter != m_listObjectPool.end(); ++iter)
		{
			s += ToString((*iter)->GetName()) + " ";
		}
	}
}
