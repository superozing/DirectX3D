#include "pch.h"
#include "CMemoryPoolMgr.h"

#include "CGameObject.h"

CMemoryPoolMgr::CMemoryPoolMgr()
	: m_vecObjectPool()
{
}

CMemoryPoolMgr::~CMemoryPoolMgr()
{
	Delete_Vec(m_vecObjectPool);
}

void CMemoryPoolMgr::init()
{
	for (int i = 0; i < 5; ++i)
	{
		CGameObject*   pObj		 = nullptr;
		Ptr<CMeshData> pMeshData = nullptr;
		pMeshData				 = CAssetMgr::GetInst()->LoadFBX(L"fbx\\TutorialTarget.fbx");

		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Target" + ToWString(std::to_string(i)));
		m_vecObjectPool.push_back(pObj);
	}
}

void CMemoryPoolMgr::tick()
{
}

CGameObject* CMemoryPoolMgr::Allocate()
{
	if (m_vecObjectPool.empty())
		return nullptr;

	else
	{
		CGameObject* obj = m_vecObjectPool.back();
		m_vecObjectPool.pop_back();
		return obj;
	}
}

void CMemoryPoolMgr::DeAllocate(CGameObject* _Object)
{
	_Object->DisconnectWithLayer();
	m_vecObjectPool.push_back(_Object);
}
