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
	CGameObject* pObj = nullptr;

	if (m_listObjectPool.size() > 0)
	{
		pObj = m_listObjectPool.front();
		m_listObjectPool.pop_front();
	}
	return pObj;
}

void CMemoryPool::PushObject(CGameObject* _Object)
{
	m_listObjectPool.push_back(_Object);
}
