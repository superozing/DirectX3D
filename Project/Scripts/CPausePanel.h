#pragma once
#include "CImageUIScript.h"

class CPausePanel : public CImageUIScript
{
private:
	bool m_bIncreaseX;
	bool m_bIncreaseY;

	Vec2 m_vIncreaseSpeed;

	vector<class CBtnUIScript*> m_vecBtns;

private:
	void Continue();
	void Exit();

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
