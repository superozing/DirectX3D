#pragma once
#include "CEventListener.h"

class CCoverObject : public CEventListener
{
private:
	bool				 m_bPlayerCover;
	CGameObject*		 m_pPlayer;
	class CPlayerScript* m_pScript;

private:
	void SetPlayerCoverRight();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CCoverObject);
	CCoverObject();
	~CCoverObject();
};
