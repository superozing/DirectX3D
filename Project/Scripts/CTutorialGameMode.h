#pragma once
#include "CGameMode.h"

enum class TutorialState
{
	BasicMove,
	Dash,
	Shooting,
	CombatFirst,
	CombatSecond,
	END,
};
class CTutorialGameMode : public CGameMode<CTutorialGameMode>
{
private:
	bool m_arrIsClear[(UINT)TutorialState::END];

public:
	bool ClearBasicMove();
	void ChangeToDash();
	bool ClearDash();
	void ChangeToShooting();
	bool ClearShooting();
	void ChangeToCombatFirst();
	bool ClearCombatFirst();
	void ChangeToCombatSecond();
	bool ClearCombatSecond();

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

	void CombatFirstBegin();
	int	 CombatFirstUpdate();
	void CombatFirstEnd();

	void CombatSecondBegin();
	int	 CombatSecondUpdate();
	void CombatSecondEnd();
#pragma endregion
};
