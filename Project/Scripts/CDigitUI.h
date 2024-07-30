#pragma once
#include "CImageUIScript.h"

class CDigitUI : public CImageUIScript
{
private:
	int m_iDigit;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CDigitUI);
	CDigitUI();
	CDigitUI(const CDigitUI& _Origin);
	~CDigitUI();
};