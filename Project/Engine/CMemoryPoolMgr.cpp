#include "pch.h"
#include "CMemoryPoolMgr.h"
#include "CMemoryPool.h"

CMemoryPoolMgr::CMemoryPoolMgr()
	: m_pMemoryPoolEX(nullptr)
{
	m_pPool = new CMemoryPool;
}

CMemoryPoolMgr::~CMemoryPoolMgr()
{
	if (m_pPool != nullptr)
		delete m_pPool;
}

void CMemoryPoolMgr::begin(wstring strPrefabRelativePath)
{
	m_pPool->begin(strPrefabRelativePath);
}

CGameObject* CMemoryPoolMgr::PopObject()
{
	return m_pPool->PopObject();
}

void CMemoryPoolMgr::PushObject(CGameObject* _Object)
{
	m_pPool->PushObject(_Object);
}

int CMemoryPoolMgr::GetCurrentCount()
{
	return m_pPool->GetCurCount();
}

void CMemoryPoolMgr::tick()
{
}
