#pragma once
#include "CProgressBar.h"

class CAmmoInfo : public CProgressBar
{
private:
	// HP Line UI
	CImageUIScript* m_pAmmoLine;

	float m_ratio;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

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

	void SetAmmoRatio(float _ratio) { m_ratio = _ratio; }

public:
	CLONE(CAmmoInfo);
	CAmmoInfo();
	~CAmmoInfo();
};
