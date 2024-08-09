#pragma once

#include <Engine\CScript.h>

class CLaserSetting : public CScript
{
private:
	float fLaserLength;
	float fAlpha;
	float fWidth;

	Vec4 LaserFrontDir;
	Vec4 LaserColor;

	Ptr<CMaterial> pLaserMtrl;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout);
	virtual void LoadFromFile(ifstream& fin);

public:
	CLONE(CLaserSetting);
	CLaserSetting();
	~CLaserSetting();
};
