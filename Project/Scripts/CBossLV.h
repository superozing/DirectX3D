﻿#pragma once
#include "CGameMode.h"

enum class BossLV_STATE
{
	FadeIn,
	OpeningIn,
	OpeningDelay,
	OpeningOut,
	PlayingIn,
	PlayingDelay,
	Playing,
	PlayingOut,
	EndingIn,
	EndingDelay,
	EndingOut,
	EndingCutIn,
	FadeOut,
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
	START_ARONA,
	WARNING,
	VICTORY_BGM,
	END,
};

#define GETBOSSLV m_BossLV->GetScript<CBossLV>()

class CBossLV : public CGameMode<CBossLV>
{
private:
	class CGameObject*	 m_Player;
	CGameObject*		 m_Kaiten;
	CGameObject*		 m_HUD;
	CGameObject*		 m_BossUI;
	class CFadeUIScript* m_FadeScript;
	class CArona*		 m_pArona;
	float				 m_FadeOutTime = 1.5f;
	float				 m_FadeInTime  = 1.5f;

	float m_OpeningInTime;
	float m_OpeningDelayTime;
	float m_OpeningOutTime;

	float m_PlayingInTime;
	float m_PlayingDelayTime;

	float m_EndingInTime;
	float m_EndingDelayTime;
	float m_EndingOutTime;

	float m_EndingCutInTime;

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

	float GetEndingInTime() { return m_EndingInTime; }
	float GetEndingDelayTime() { return m_EndingDelayTime; }
	float GetEndingOutTime() { return m_EndingOutTime; }

	float GetEndingCutInTime() { return m_EndingCutInTime; }

	float GetAccTime() { return m_Acctime; }

public:
	void LoadSoundAsset();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CBossLV);
	CBossLV();
	CBossLV(const CBossLV& _Origin);
	~CBossLV();

private:
	void FadeInBegin();
	int	 FadeInUpdate();
	void FadeInEnd();

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

	void EndingInBegin();
	int	 EndingInUpdate();
	void EndingInEnd();

	void EndingDelayBegin();
	int	 EndingDelayUpdate();
	void EndingDelayEnd();

	void EndingOutBegin();
	int	 EndingOutUpdate();
	void EndingOutEnd();

	void EndingCutInBegin();
	int	 EndingCutInUpdate();
	void EndingCutInEnd();

	void FadeOutBegin();
	int	 FadeOutUpdate();
	void FadeOutEnd();

	friend class CBossOpeningTex;
	friend class CStartTex;
	friend class CBossScript;
};
