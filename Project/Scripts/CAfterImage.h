#pragma once
#include <Engine\CScript.h>

class CAfterImage : public CScript
{
private:
	AfterImageInfo m_info;
	float		   fUpdateTimer;
	bool		   bDisplayNode;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void UpdateData() override;
	virtual void Clear() override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CAfterImage);
	CAfterImage();
	~CAfterImage();
};
