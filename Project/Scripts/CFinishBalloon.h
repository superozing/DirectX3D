#pragma once
#include <Engine\CScript.h>
class CFinishBalloon : public CScript
{
private:
	float m_fUpLength;
	float m_fUpTime;
	float m_fWaveLength;
	float m_fWaveTime;

	bool  m_bUp;
	float m_fAccTime;
	float m_fWaveAccTime;

public:
	void Up();
	void Wave();

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout);
	virtual void LoadFromFile(ifstream& fin);

public:
	CLONE(CFinishBalloon);
	CFinishBalloon();
	~CFinishBalloon();
};
