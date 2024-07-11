#include "pch.h"
#include "CMemoryPoolMgr.h"
#include "CMemoryPool.h"

#define TagPoolCount "[CurrentPoolCount]"
#define TagMemoryPoolPrefab "[Pool Prefab]"
#define TagMemoryPoolCount "[PoolCount]"

CMemoryPoolMgr::CMemoryPoolMgr()
	: m_pMemoryPoolEX(nullptr)
{
	m_pPool = new CMemoryPool;
}

CMemoryPoolMgr::~CMemoryPoolMgr()
{
	if (m_pPool != nullptr)
		delete m_pPool;

	Delete_Map<string, CMemoryPool*>(m_mapMemoryPool);
}

void CMemoryPoolMgr::begin(wstring strPrefabRelativePath)
{
	m_pPool->begin(strPrefabRelativePath);

	wstring PoolBeginPath = CPathMgr::GetLogPath();
	PoolBeginPath += L"\\PoolInit.txt";

	if (!exists(PoolBeginPath))
	{
		return;
	}

	ifstream fin(PoolBeginPath);

	if (!fin.is_open())
		MessageBox(nullptr, L"Poolinit Info Load Fail", L"Pool begin 실패", 0);

	int iPoolNum;
	Utils::GetLineUntilString(fin, TagPoolCount);
	fin >> iPoolNum;

	for (int i = 0; i < iPoolNum; ++i)
	{
		// Pool Key
		string strPoolKey;

		Utils::GetLineUntilString(fin, TagMemoryPoolPrefab);
		fin >> strPoolKey;

		// Pool Count
		int iPoolCount;
		Utils::GetLineUntilString(fin, TagMemoryPoolCount);
		fin >> iPoolCount;

		CMemoryPool* pPool = new CMemoryPool;
		pPool->begin(ToWString(strPoolKey)); // iPoolCount
		m_mapMemoryPool.insert(make_pair(strPoolKey, pPool));
	}
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

vector<std::pair<string, int>> CMemoryPoolMgr::GetPoolKeys()
{
	vector<std::pair<string, int>> res;

	for (auto iter = m_mapMemoryPool.begin(); iter != m_mapMemoryPool.end(); ++iter)
	{
		std::pair<string, int> p;
		p.first	 = (iter->first);
		p.second = iter->second->GetMaxCount();

		res.push_back(p);
	}

	return res;
}

void CMemoryPoolMgr::SavePool()
{
	wstring strPoolSavePath = CPathMgr::GetLogPath();
	strPoolSavePath += L"\\PoolInit.txt";

	// 메모리 풀 초기세팅 저장
	ofstream fPoolout(strPoolSavePath, ofstream::out | ofstream::trunc);

	if (!fPoolout.is_open())
	{
		MessageBox(nullptr, L"MemoryPool Save fail!", L"Client 종료 에러", 0);
	}

	vector<std::pair<string, int>> PoolKey = CMemoryPoolMgr::GetInst()->GetPoolKeys();

	fPoolout << TagPoolCount << endl;
	fPoolout << PoolKey.size() << endl;

	for (int i = 0; i < PoolKey.size(); ++i)
	{
		std::pair<string, int> p = PoolKey[i];

		fPoolout << TagMemoryPoolPrefab << endl;
		fPoolout << p.first << endl;

		fPoolout << TagMemoryPoolCount << endl;
		fPoolout << p.second << endl;

		fPoolout << endl;
	}

	fPoolout.close();
}

CMemoryPool* CMemoryPoolMgr::FindPool(string _strMapKey)
{
	map<string, CMemoryPool*>::iterator iter = m_mapMemoryPool.find(_strMapKey);

	if (iter == m_mapMemoryPool.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CMemoryPoolMgr::tick()
{
}
