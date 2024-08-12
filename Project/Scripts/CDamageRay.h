#pragma once

#include <Engine\CScript.h>

class CDamageRay : public CScript
{
private:
	Vec4  LaserColor;
	float Length;
	float Alpha;
	float Width;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

	virtual void UpdateData() override;
	virtual void Clear() override;

public:
	CLONE(CDamageRay);
	CDamageRay();
	CDamageRay(const CDamageRay& _Other);
	~CDamageRay();
};
