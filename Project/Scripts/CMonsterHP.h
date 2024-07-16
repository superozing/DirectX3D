#pragma once
#include "CProgressBar.h"
class CMonsterHP : public CProgressBar
{
private:
	// HP Line UI
	CImageUIScript* m_pHPLineUI;

public:
	virtual void begin() override;
	virtual void tick() override;

private:
	virtual void MakeChildObjects() override;

public:
	void SetMaxHP(int _MaxHP) { CProgressBar::SetMaxValue(RoRMath::ClampInt(_MaxHP, 0)); }
	void SetCurHP(int _CurHP) { CProgressBar::SetCurValue(RoRMath::ClampInt(_CurHP, 0, CProgressBar::GetMaxValue())); }

	int GetMaxHP() const { return CProgressBar::GetMaxValue(); }
	int GetCurHP() const { return CProgressBar::GetCurValue(); }

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CMonsterHP);
	CMonsterHP();
	~CMonsterHP();
};
