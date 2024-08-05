#pragma once
#include <Engine\CScript.h>
class CButtons : public CScript
{
private:
	int							m_iIdx;
	vector<class CBtnUIScript*> m_vecButtons;

public:
	void AddIdx();
	void SubIdx();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CButtons);
	CButtons();
	~CButtons();
};
