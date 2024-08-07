#pragma once
#include "CGameMode.h"

enum class TutorialState
{
	BasicMove,
	Dash,
	Shooting,
	CoverHigh,
	CoverJump,
	CoverLow,
	Ending,
	END,
};
class CTutorialGameMode : public CGameMode<CTutorialGameMode>
{
private:
	class CArona*	   m_pArona;
	class CGameObject* m_pWall;

	Ptr<CSound> m_pBGM;

private:
	bool m_arrIsClear[(UINT)TutorialState::END];

public:
	void Clear(TutorialState _state);
	bool IsClear(TutorialState _state);

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CTutorialGameMode();
	~CTutorialGameMode();

#pragma region FSM
	void BasicMoveBegin();
	int	 BasicMoveUpdate();
	void BasicMoveEnd();

	void DashBegin();
	int	 DashUpdate();
	void DashEnd();

	void ShootingBegin();
	int	 ShootingUpdate();
	void ShootingEnd();

	void CoverHighBegin();
	int	 CoverHighUpdate();
	void CoverHighEnd();

	void CoverJumpBegin();
	int	 CoverJumpUpdate();
	void CoverJumpEnd();

	void CoverLowBegin();
	int	 CoverLowUpdate();
	void CoverLowEnd();

	void EndingBegin();
	int	 EndingUpdate();
	void EndingEnd();
#pragma endregion
};
