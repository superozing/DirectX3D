#pragma once
#include "CGameMode.h"
#include "CPlayerController.h"
enum class TutorialState
{
	First,
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
	Ending,
	END,
};

enum class TutorialGameModeSoundType
{
	TutorialStart,
	BGM,
	DoorOpen,
	END
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
#define TurretSpawnCNT 2
class CTutorialGameMode : public CGameMode<CTutorialGameMode>
{
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
	vector<class CTurret*> m_vecCoverMonsters;

	// Finish
private:
	class CFinishBalloon* m_pFinishBalloon;

private:
	class CArona*		 m_pArona;
	class CGameObject*	 m_pWall;
	class CPlayerScript* m_pPlayerScript;
	class CGameObject*	 m_pPlayer;

	class CEventListener* m_pEvents[(UINT)TutorialEvents::END];

	Ptr<CSound>			m_pBGM;
	vector<Ptr<CSound>> m_vecTutorialGameModeSound;

private:
	bool m_arrIsClear[(UINT)TutorialEvents::END];

public:
	void Clear(TutorialEvents _state);
	bool IsClear(TutorialEvents _state);

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CTutorialGameMode();
	~CTutorialGameMode();

#pragma region FSM
	void FirstBegin();
	int	 FirstUpdate();
	void FirstEnd();

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

	void EndingBegin();
	int	 EndingUpdate();
	void EndingEnd();
#pragma endregion
};
