#pragma once
#include "CGameMode.h"

enum class BossLV_STATE
{
	OpeningIn,
	OpeningDelay,
	OpeningOut,
	PlayingIn,
	PlayingDelay,
	Playing,
	PlayingOut,
	Ending,
	END,
};

enum class BossLV_BGM
{
	KAITEN_SCREW,
	PARTY_TIME,
	END,
};

enum class BossLV_SFX
{
	START,
	WARNING,
	END,
};

#define GETBOSSLV m_BossLV->GetScript<CBossLV>()

class CBossLV : public CGameMode<CBossLV>
{
private:
	class CGameObject* m_Player;
	CGameObject*	   m_HUD;

	float m_OpeningInTime;
	float m_OpeningDelayTime;
	float m_OpeningOutTime;

	float m_PlayingInTime;
	float m_PlayingDelayTime;

	float m_Acctime;

	vector<Ptr<CSound>> m_BGM;
	vector<Ptr<CSound>> m_SFX;

public:
	int GetCurLVState();

	float GetOpeningInTime() { return m_OpeningInTime; }
	float GetOpeningDelayTime() { return m_OpeningDelayTime; }
	float GetOpeningOutTime() { return m_OpeningOutTime; }

	float GetPlayingInTime() { return m_PlayingInTime; }
	float GetPlayingDelayTime() { return m_PlayingDelayTime; }

	float GetAccTime() { return m_Acctime; }

public:
	void LoadSoundAsset();

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

	void PlayingInBegin();
	int	 PlayingInUpdate();
	void PlayingInEnd();

	void PlayingDelayBegin();
	int	 PlayingDelayUpdate();
	void PlayingDelayEnd();

	void PlayingBegin();
	int	 PlayingUpdate();
	void PlayingEnd();

	void PlayingOutBegin();
	int	 PlayingOutUpdate();
	void PlayingOutEnd();

	void EndingBegin();
	int	 EndingUpdate();
	void EndingEnd();

	friend class CBossOpeningTex;
	friend class CStartTex;
	friend class CBossScript;
};
