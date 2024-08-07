#pragma once

#include <Engine\CScript.h>

class CDamageRay : public CScript
{
public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CDamageRay);
	CDamageRay();
	CDamageRay(const CDamageRay& _Other);
	~CDamageRay();
};
