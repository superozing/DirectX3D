#pragma once
#include "CProgressBar.h"

class CAmmoInfo : public CProgressBar
{
private:
	// HP Line UI
	CImageUIScript* m_pAmmoLine;

public:
	virtual void begin() override;
	virtual void tick() override;

private:
	virtual void MakeChildObjects() override;

public:
	void SetMaxAmmo(int _MaxHP) { CProgressBar::SetMaxValue(RoRMath::ClampInt(_MaxHP, 0)); }
	void SetCurAmmo(int _CurHP)
	{
		CProgressBar::SetCurValue(RoRMath::ClampInt(_CurHP, 0, CProgressBar::GetMaxValue()));
	}

	int GetMaxAmmo() const { return CProgressBar::GetMaxValue(); }
	int GetCurAmmo() const { return CProgressBar::GetCurValue(); }

public:
	CLONE(CAmmoInfo);
	CAmmoInfo();
	~CAmmoInfo();
};
