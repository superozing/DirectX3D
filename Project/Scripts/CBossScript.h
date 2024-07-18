﻿#pragma once
#include <Engine\CScript.h>

enum class BOSS_STATE
{
	Ready1,
	Ready2,
	EXs1,
	EXs2,
	EXs3,
	EXs4,
	EXs5,
	NormalAttackDelay,
	NormalAttackIng,
	NormalAttackEnd,
	NormalAttackStart,
	NormalIdle,
	NormalReload,
	VitalDeath,
	VitalGroggy,
	VitalGroggyDeath,

	END,
};

enum class BOSS_GAMESTATE
{
	BATTLEREADY,
	IDLE,
	ATTACK,
	EXS,
	GROGGY,
	DEATH,
	GROGGYDEATH,

	END,
};

struct tBossStatus
{
	float MaxHP = 1000.f;
	float CurHP = MaxHP;

	float ATTDamage = 10.f;
	float ATTSpeed = 1.f;

	float EXsCoolTime = 5.f;

	bool IsDead = false;
};

template <typename T> class CRoRStateMachine;

class CBossScript : public CScript
{
private:
	CRoRStateMachine<CBossScript>* m_FSM;
	tBossStatus					   m_BossStatus;

	float m_AttDuration;
	float m_EXsDuration;

	bool m_ActiveAttack;
	bool m_ActiveEXs;

	BOSS_GAMESTATE m_GameState;

#pragma region About State
public:
	void NormalIdleBegin();
	int	 NormalIdleUpdate();
	void NormalIdleEnd();

	void NormalReloadBegin();
	int	 NormalReloadUpdate();
	void NormalReloadEnd();

	void NormalAttackStartBegin();
	int	 NormalAttackStartUpdate();
	void NormalAttackStartEnd();	
	void NormalAttackIngBegin();
	int	 NormalAttackIngUpdate();
	void NormalAttackIngEnd();
	void NormalAttackDelayBegin();
	int	 NormalAttackDelayUpdate();
	void NormalAttackDelayEnd();
	void NormalAttackEndBegin();
	int	 NormalAttackEndUpdate();
	void NormalAttackEndEnd();

	void EXs1Begin();
	int	 EXs1Update();
	void EXs1End();
	void EXs2Begin();
	int	 EXs2Update();
	void EXs2End();
	void EXs3Begin();
	int	 EXs3Update();
	void EXs3End();
	void EXs4Begin();
	int	 EXs4Update();
	void EXs4End();
	void EXs5Begin();
	int	 EXs5Update();
	void EXs5End();

	void VitalDeathBegin();
	int	 VitalDeathUpdate();
	void VitalDeathEnd();
	void VitalGroggyBegin();
	int	 VitalGroggyUpdate();
	void VitalGroggyEnd();
	void VitalGroggyDeathBegin();
	int	 VitalGroggyDeathUpdate();
	void VitalGroggyDeathEnd();

	void Ready1Begin();
	int	 Ready1Update();
	void Ready1End();
	void Ready2Begin();
	int	 Ready2Update();
	void Ready2End();
#pragma endregion

public:
	void CheckDuration();
	void CheckNormalAttack();
	void CheckEXs();
	void CheckVital();

	void		   SetBossGameState(BOSS_GAMESTATE _state) { m_GameState = _state; }
	BOSS_GAMESTATE GetBossGameState() { return m_GameState; }

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CBossScript);
	CBossScript();
	~CBossScript();

private:
	void InitStateMachine();
	void InitScriptParamUI();
};
