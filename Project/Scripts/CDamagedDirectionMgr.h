#pragma once
#include <Engine/CScript.h>

class CDamagedDirection;

// 피격 방향 UI를 관리하기 위한 스크립트
class CDamagedDirectionMgr : public CScript
{
private:
	UINT m_iDirectionCnt;

	// 피격 방향 UI 배열
	vector<CDamagedDirection*> m_arrDamagedDirectionUI;

	// 현재 조종할 피격 방향 UI의 인덱스 (0 ~ 2)
	UINT m_Idx;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

private:
	void SetParentPanelUI();
	void AddIdx() { m_Idx = m_Idx == m_iDirectionCnt - 1 ? 0 : m_Idx + 1; }

public:
	void AddDamagedDirection(Vec3 _EnemyPos, float _RadiusRatio);

public:
	CLONE(CDamagedDirectionMgr);
	CDamagedDirectionMgr();
	CDamagedDirectionMgr(const CDamagedDirectionMgr& _Origin);
	~CDamagedDirectionMgr();
};