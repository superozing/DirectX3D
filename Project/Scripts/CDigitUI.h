#pragma once
#include "CImageUIScript.h"

class CDigitUI : public CImageUIScript
{
private:
	int m_iDigit;

public:
	void SetNum(int _digit) { m_iDigit = _digit; }

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	void SetValue(int _iVal) { m_iDigit = _iVal; }

public:
	CLONE(CDigitUI);
	CDigitUI();
	CDigitUI(const CDigitUI& _Origin);
	~CDigitUI();
};