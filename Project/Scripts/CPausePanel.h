#pragma once
#include "CImageUIScript.h"

class CPausePanel : public CImageUIScript
{
private:
	bool m_bOpening;
	Vec2 m_vOpenSpeed;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CPausePanel);
	CPausePanel();
	~CPausePanel();
};
