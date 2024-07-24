#pragma once
#include "CEventListener.h"

class CCoverArea : public CEventListener
{
private:
	bool				  m_bPlayerCover;
	bool				  m_bStand;
	CGameObject*		  m_pPlayer;
	class CPlayerScript*  m_pScript;
	class CImageUIScript* m_pUI;
	class CCoverUI*		  m_UIMgr;

private:
	void SetPlayerCover();

public:
	void PrintUI(bool _print);

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CCoverArea);
	CCoverArea();
	~CCoverArea();
};
