#pragma once
#include "CTextUI.h"

class CAmmoInfo : public CTextUI
{
private:
	int m_iCurAmmo;
	int m_iMaxAmmo;

	class CDigitUI*				 m_pCurAmmo;
	class CDigitUI*				 m_pMaxAmmo;
	class CShootingSystemScript* m_pSS;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CAmmoInfo);
	CAmmoInfo();
	CAmmoInfo(const CAmmoInfo& _Origin);
	~CAmmoInfo();
};
