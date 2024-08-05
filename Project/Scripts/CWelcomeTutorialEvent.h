#pragma once
#include "CEventListener.h"

#include "CPlayerController.h"
class CWelcomeTutorialEvent : public CEventListener
{
private:
	bool m_bStart;

	float m_fStopTimer;
	float m_fStopTimeLength;
	bool  m_bWalkAllow;

	bool m_bMoveFront;
	bool m_bMoveLeft;
	bool m_bMoveBack;
	bool m_bMoveRight;

	bool  m_bWalkEnough;
	float m_fTargetDistance;

	PlayerKeyInfo m_tPlayerKeyInfo;

	class CArona*	   m_pArona;
	class CGameObject* m_pPlayer;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CWelcomeTutorialEvent);
	CWelcomeTutorialEvent();
	~CWelcomeTutorialEvent();
};
