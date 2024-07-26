#pragma once
#include <Engine/CUIScript.h>

class CAmmoInfo : public CUIScript
{
private:
	tFontInfo m_tAmmoFont;

	bool m_bDraw;

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
