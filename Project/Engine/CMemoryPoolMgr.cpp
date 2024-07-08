#include "pch.h"
#include "CMemoryPoolMgr.h"

#include "CGameObject.h"

CMemoryPoolMgr::CMemoryPoolMgr()
	: m_listObjectPool()
	, iPoolMaxCount(5)
	, iCurPopCount(0)
{
}

CMemoryPoolMgr::~CMemoryPoolMgr()
{
	Delete_List(m_listObjectPool);
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
		m_listObjectPool.push_back(pObj);
	}
}

void CMemoryPoolMgr::tick()
{
}

#include "CLogMgr.h"
CGameObject* CMemoryPoolMgr::PopObject()
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

void CMemoryPoolMgr::PushObject(CGameObject* _Object)
{
	_Object->DisconnectWithLayer();
	m_listObjectPool.push_back(_Object);
	--iCurPopCount;

	string s = std::to_string(iCurPopCount);
	CLogMgr::GetInst()->AddLog(Log_Level::WARN, s);

	if (iCurPopCount == 0)
	{
		m_listObjectPool.sort([](CGameObject* a, CGameObject* b) { return a->GetName() < b->GetName(); });

		string s;

		for (auto iter = m_listObjectPool.begin(); iter != m_listObjectPool.end(); ++iter)
		{
			s += ToString((*iter)->GetName()) + " ";
		}

		CLogMgr::GetInst()->AddLog(Log_Level::INFO, s);
	}
}
