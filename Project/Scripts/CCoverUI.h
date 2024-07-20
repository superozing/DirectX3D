#pragma once
#include "CEventListener.h"
class CCoverUI : public CEventListener
{
private:
	class CPlayerScript* m_pPlayerScript;
	CGameObject*		 m_pUI;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CCoverUI);
	CCoverUI();
	~CCoverUI();
};
