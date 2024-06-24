#pragma once
#include "singleton.h"

#include <random>

class CRandomMgr : public CManager<CRandomMgr>
{
private:
	UINT32							m_seed;
	std::mt19937					m_GenEngine;
	std::uniform_int_distribution<> m_DistGenerator;

	wstring m_strPrint;

public:
	/// <summary>
	/// 랜덤한 시드를 생성합니다.
	/// </summary>
	virtual void init() override;

	virtual void enter() override {}

	/// <summary>
	/// 랜덤한 값을 추출합니다
	/// </summary>
	UINT GetRandomInt();

	/// <summary>
	/// 랜덤한 값을 추출합니다 _bound를 포함하지 않습니다.
	/// </summary>
	/// <param name="_bound">추출하고싶은 최대 값</param>
	/// <returns> 0을 포함해서부터  _bound 포함하지 않는 까지 랜덤한 값을 반환합니다 </returns>
	UINT GetRandomInt(UINT _bound);

	/// <summary>
	/// 바운더리 내의 랜덤한 값을 추출합니다. 최대값을 포함하지않습니다
	/// </summary>
	/// <param name="_low">최솟값</param>
	/// <param name="_high">최댓값</param>
	/// <returns>랜덤값</returns>
	UINT GetRandomInt(UINT _low, UINT _high);

	/// <summary>
	/// _probability/_bound의 확률로 성공과 실패 여부를 반환합니다.
	/// </summary>
	/// <param name="_probability">확률의 분자</param>
	/// <param name="_bound">확률의 분모</param>
	/// <returns>성공(true), 실패(false)</returns>
	bool GetRandomSucceed(UINT _probability, UINT _bound);

	/// <summary>
	/// [-1, 1]로 정규화된 랜덤값을 얻습니다.
	/// </summary>
	float GetRandomFloat();

	/// <summary>
	/// 바운더리 내의 랜덤 실수 값을 얻습니다.
	/// </summary>
	/// <param name="_low">최솟값</param>
	/// <param name="_high">최댓값</param>
	float GetRandomFloat(float _low, float _high);

	/// <summary>
	/// 배열의 요소의 한개를 랜덤으로 제거하고 반환합니다. 제거된 요소는 반환됩니다.
	/// </summary>
	template <typename T> T PopRandom(vector<T>& vec);

	/// <summary>
	/// 리스트의 요소의 한개를 랜덤으로 제거하고 반환합니다. 제거된 요소는 반환됩니다.
	/// </summary>
	template <typename T> T PopRandom(list<T>& _list);

	/// <summary>
	/// 배열 안에 들어있는 요소중 하나를 얻습니다. 원본을 훼손하지 않습니다.
	/// </summary>
	template <typename T> T GetRandomElement(const vector<T>& vec);

	/// <summary>
	/// 리스트 안에 들어있는 요소중 하나를 얻습니다. 원본을 훼손하지 않습니다.
	/// </summary>
	template <typename T> T GetRandomElement(const list<T>& _list);

	/// <summary>
	/// 배열을 랜덤하게 섞어줍니다
	/// </summary>
	template <typename T> void SortRandom(vector<T>& vec);

	/// <summary>
	/// 리스트를 랜덤하게 섞어줍니다
	/// </summary>
	template <typename T> void SortRandom(list<T>& _list);

	UINT32 GetSeed() { return m_seed; }
	void   SetSeed(UINT32 _seed);

public:
	void Test();

public:
	CRandomMgr();
	~CRandomMgr();
};

template <typename T> inline T CRandomMgr::PopRandom(vector<T>& vec)
{
	int idx	 = GetRandomInt(vec.size());
	T	data = vec[idx];
	vec.erase(vec.begin() + idx);
	return data;
}

template <typename T> inline T CRandomMgr::PopRandom(list<T>& li)
{
	int idx	 = GetRandomInt(li.size());
	T	data = li[idx];
	li.erase(li.begin() + idx);
	return data;
}

template <typename T> inline T CRandomMgr::GetRandomElement(const vector<T>& vec)
{
	int idx	 = GetRandomInt(vec.size());
	T	data = vec[idx];
	return data;
}

template <typename T> inline T CRandomMgr::GetRandomElement(const list<T>& li)
{
	int idx	 = GetRandomInt(li.size());
	T	data = li[idx];
	return data;
}

template <typename T> inline void CRandomMgr::SortRandom(vector<T>& vec)
{
	vector<T> other;

	size_t size = vec.size();
	for (size_t i = 0; i < size; i++)
	{
		other.push_back(PopRandom(vec));
	}

	other.swap(vec);
}

template <typename T> inline void CRandomMgr::SortRandom(list<T>& li)
{
	list<T> other;

	size_t size = li.size();
	for (size_t i = 0; i < size; i++)
	{
		other.push_back(PopRandom(li));
	}

	other.swap(li);
}
