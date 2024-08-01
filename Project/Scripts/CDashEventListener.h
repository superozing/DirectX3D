#pragma once
#include "CEventListener.h"
class CDashEventListener : public CEventListener
{
private:
	bool				 m_bFirstFrm;
	UINT				 m_iDashCnt;
	class CPlayerScript* m_pPlayerScript;
	class CArona*		 m_pArona;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CDashEventListener);
	CDashEventListener();
	~CDashEventListener();
};
