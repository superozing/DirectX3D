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
	void SetMaxHP(int _MaxHP) { CProgressBar::SetMaxHP(RoRMath::ClampInt(_MaxHP, 0)); }
	void SetCurHP(int _CurHP) { CProgressBar::SetCurHP(RoRMath::ClampInt(_CurHP, 0, CProgressBar::GetMaxHP())); }

	int GetMaxHP() const { return CProgressBar::GetMaxHP(); }
	int GetCurHP() const { return CProgressBar::GetCurHP(); }

public:
	CLONE(CMonsterHP);
	CMonsterHP();
	~CMonsterHP();
};
