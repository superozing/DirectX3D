#pragma once
#include "CGameMode.h"

enum class BossLV_STATE
{
	OpeningIn,
	OpeningDelay,
	OpeningOut,
	Playing,
	Ending,
	END,
};

class CBossLV : public CGameMode<CBossLV>
{
private:
	float m_OpeningInTime;
	float m_OpeningDelayTime;
	float m_OpeningOutTime;
	float m_Acctime;

public:
	int GetCurLVState();

	float GetOpeningInTime() { return m_OpeningInTime; }
	float GetOpeningDelayTime() { return m_OpeningDelayTime; }
	float GetOpeningOutTime() { return m_OpeningOutTime; }
	float GetAccTime() { return m_Acctime; }

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CBossLV);
	CBossLV();
	~CBossLV();

private:
	void OpeningInBegin();
	int	 OpeningInUpdate();
	void OpeningInEnd();

	void OpeningDelayBegin();
	int	 OpeningDelayUpdate();
	void OpeningDelayEnd();

	void OpeningOutBegin();
	int	 OpeningOutUpdate();
	void OpeningOutEnd();

	void PlayingBegin();
	int	 PlayingUpdate();
	void PlayingEnd();

	void EndingBegin();
	int	 EndingUpdate();
	void EndingEnd();

	friend class CBossOpeningTex;
	friend class CStartTex;
};
