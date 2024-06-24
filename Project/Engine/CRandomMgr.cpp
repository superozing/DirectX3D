#include "pch.h"
#include "CRandomMgr.h"

CRandomMgr::CRandomMgr()
{
}

CRandomMgr::~CRandomMgr()
{
}

void CRandomMgr::SetSeed(UINT32 _seed)
{
	// 난수 생성 엔진 초기화를 위한 시드 생성
	m_seed = _seed;
	std::mt19937 gen(m_seed); // Mersenne Twister 난수 생성 엔진 초기화
	m_GenEngine = gen;
	std::uniform_int_distribution<> dis(0, INT_MAX - 1);
	m_DistGenerator = dis;
}

void CRandomMgr::init()
{
	std::random_device rd; // 하드웨어 난수 생성기를 사용하여 시드 생성
	SetSeed(rd());
}

UINT CRandomMgr::GetRandomInt()
{
	return m_DistGenerator(m_GenEngine);
}

UINT CRandomMgr::GetRandomInt(UINT _bound)
{
	return m_DistGenerator(m_GenEngine) % _bound;
}

UINT CRandomMgr::GetRandomInt(UINT _low, UINT _high)
{
	if (_low == _high)
		return _low;
	if (_low > _high)
	{
		UINT temp = _high;
		_high	  = _low;
		_low	  = temp;
	}

	UINT bound = _high - _low;

	return m_DistGenerator(m_GenEngine) % bound + _low;
}

bool CRandomMgr::GetRandomSucceed(UINT _probability, UINT _bound)
{
	return GetRandomInt(_bound) < _probability ? true : false;
}

float CRandomMgr::GetRandomFloat()
{
	UINT  rand	= GetRandomInt();
	float fRand = (float)rand / INT_MAX;
	fRand		= 2.0f * fRand - 1.0f;

	return fRand;
}

float CRandomMgr::GetRandomFloat(float _low, float _high)
{
	if (_low == _high)
		return _low;
	if (_low > _high)
	{
		float temp = _high;
		_high	   = _low;
		_low	   = temp;
	}

	UINT  rand	= GetRandomInt();
	float fRand = (float)rand / INT_MAX;
	fRand		= (_high - _low) * fRand + _low;

	return fRand;
}

void CRandomMgr::Test()
{
	MessageBox(nullptr, L"Random Manager Test Execute", L"Test Code", MB_OK);

	init();

	auto seed = GetSeed();

	SetSeed(seed - 1);

	auto setSeed = GetSeed();

	int TestCnt = 10000;

	for (int i = 0; i < TestCnt; i++)
	{
		UINT rand = GetRandomInt();
	}

	for (int i = 0; i < TestCnt; i++)
	{
		UINT rand = GetRandomInt(100);
		if (rand < 0 || rand >= 100)
			assert(0);
	}

	for (int i = 0; i < TestCnt; i++)
	{
		UINT rand = GetRandomInt(40, 60);
		if (rand < 40 || rand >= 60)
			assert(0);
	}

	vector<bool> vecSucceedTest;
	for (int i = 0; i < TestCnt; i++)
	{
		bool succeed = GetRandomSucceed(1, 5);
		vecSucceedTest.push_back(succeed);
	}

	for (int i = 0; i < TestCnt; i++)
	{
		float rand = GetRandomFloat();
		if (rand < -1.f || rand > 1.f)
			assert(0);
	}

	for (int i = 0; i < TestCnt; i++)
	{
		float rand = GetRandomFloat(3.f, 11.f);
		if (rand < 3.f || rand > 11.f)
			assert(0);
	}

	vector<int> vecInts;
	for (int i = 0; i < 100; i++)
		vecInts.push_back(i);

	int randomInt = GetRandomElement(vecInts);

	SortRandom(vecInts);

	for (int i = 0; i < 100; i++)
	{
		PopRandom(vecInts);
	}

	vector<int> listInts;
	for (int i = 0; i < 100; i++)
		listInts.push_back(i);

	randomInt = GetRandomElement(listInts);

	SortRandom(listInts);

	for (int i = 0; i < 100; i++)
	{
		PopRandom(listInts);
	}
}
