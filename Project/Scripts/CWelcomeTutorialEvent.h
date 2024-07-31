#pragma once
#include "CEventListener.h"

#include "CPlayerController.h"
class CWelcomeTutorialEvent : public CEventListener
{
private:
	float m_fStopTimer;
	float m_fStopTimeLength;
	bool  m_bWalkAllow;

	bool m_bTabW;
	bool m_bTabA;
	bool m_bTabS;
	bool m_bTabD;

	bool m_bWalkEnough;

	PlayerKeyInfo m_tPlayerKeyInfo;

	class CAtlasImageUIScript* m_pArona;

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
