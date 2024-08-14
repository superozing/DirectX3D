#pragma once
#include "CGameMode.h"
#include "CPlayerController.h"
enum class TutorialState
{
	FadeIn,
	OpeningIn,
	OpeningDelay,
	OpeningOut,
	BasicMove,
	DashWait,
	Dash,
	ShootingWait,
	Shooting,
	CoverHighWait,
	CoverHigh,
	CoverJumpWait,
	CoverJump,
	CoverLowWait,
	CoverLow,
	EndingWait,
	EndingIn,
	EndingDelay,
	EndingOut,
	EndingCutIn,
	FadeOut,
	END,
};

enum class TutorialGameModeSoundType
{
	TutorialStart,
	BGM,
	DoorOpen,
	MissionStart1,
	MissionStart2,
	MissionClear,
	VICTORY_BGM,
	END,
};

enum class TutorialEvents
{
	BasicMove,
	Dash,
	Shooting,
	CoverHigh,
	CoverJump,
	CoverLow,
	Ending,
	END
};

#define SPAWNERCNT 5
#define TURRETSPAWNCNT 2
#define DroidARPath "prefab\\Monster\\DroidAR.pref"
class CTutorialGameMode : public CGameMode<CTutorialGameMode>
{
	// Opening
private:
	float m_FadeInTime	= 3.f;
	float m_FadeOutTime = 3.f;
	float m_Acctime;
	float m_OpeningInTime;
	float m_OpeningDelayTime;
	float m_OpeningOutTime;

public:
	float GetOpeningInTime() { return m_OpeningInTime; }
	float GetAccTime() { return m_Acctime; }
	float GetOpeningOutTime() { return m_OpeningOutTime; }

	// BasicMoveState
private:
	float		  m_fStopTimeLength;
	float		  m_fStopTimer;
	bool		  m_bMoveFront;
	bool		  m_bMoveBack;
	bool		  m_bMoveLeft;
	bool		  m_bMoveRight;
	float		  m_fTargetDistance;
	PlayerKeyInfo m_tPlayerKeyInfo;
	Vec3		  PrevPos;

	// DashState
private:
	int m_iDashCnt;

	// ShootingState
private:
	bool							m_arrIsMonsterDestroy[SPAWNERCNT];
	vector<class CSpawnSpotScript*> m_vecTargetSpawners;
	vector<class CTutorialTarget*>	m_vecTutorialTargets;

	// CoverHigh
private:
	vector<class CGameObject*> m_vecCoverMonsters;

	// CoverLow
private:
	vector<class CGameObject*> m_vecCoverLowMonsters;

	// Ending
private:
	class CFinishBalloon* m_pFinishBalloon;
	float				  m_EndingInTime;
	float				  m_EndingDelayTime;
	float				  m_EndingOutTime;
	float				  m_EndingCutInTime;

public:
	float GetEndingInTime() { return m_EndingInTime; }
	float GetEndingOutTime() { return m_EndingOutTime; }

private:
	class CArona*		 m_pArona;
	class CGameObject*	 m_pWall;
	class CPlayerScript* m_pPlayerScript;
	class CGameObject*	 m_pPlayer;
	class CHUD*			 m_pHUD;
	class CFadeUIScript* m_FadeScript;

	class CEventListener* m_pEvents[(UINT)TutorialEvents::END];

	Ptr<CSound>			m_pBGM;
	vector<Ptr<CSound>> m_vecTutorialGameModeSound;

private:
	bool m_arrIsClear[(UINT)TutorialEvents::END];

public:
	void Clear(TutorialEvents _state);
	bool IsClear(TutorialEvents _state);

	int GetCurState() { return m_FSM->GetCurState(); }

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CTutorialGameMode();
	~CTutorialGameMode();

#pragma region FSM
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

	void BasicMoveBegin();
	int	 BasicMoveUpdate();
	void BasicMoveEnd();

	void DashWaitBegin();
	int	 DashWaitUpdate();
	void DashWaitEnd();

	void DashBegin();
	int	 DashUpdate();
	void DashEnd();

	void ShootingWaitBegin();
	int	 ShootingWaitUpdate();
	void ShootingWaitEnd();

	void ShootingBegin();
	int	 ShootingUpdate();
	void ShootingEnd();

	void CoverHighWaitBegin();
	int	 CoverHighWaitUpdate();
	void CoverHighWaitEnd();

	void CoverHighBegin();
	int	 CoverHighUpdate();
	void CoverHighEnd();

	void CoverJumpWaitBegin();
	int	 CoverJumpWaitUpdate();
	void CoverJumpWaitEnd();

	void CoverJumpBegin();
	int	 CoverJumpUpdate();
	void CoverJumpEnd();

	void CoverLowWaitBegin();
	int	 CoverLowWaitUpdate();
	void CoverLowWaitEnd();

	void CoverLowBegin();
	int	 CoverLowUpdate();
	void CoverLowEnd();

	void EndingWaitBegin();
	int	 EndingWaitUpdate();
	void EndingWaitEnd();

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

#pragma endregion
};
