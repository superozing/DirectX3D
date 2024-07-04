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

public:
	CLONE(CMonsterHP);
	CMonsterHP();
	~CMonsterHP();
};
