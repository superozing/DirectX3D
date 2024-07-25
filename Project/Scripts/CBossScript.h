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

struct tBossStatus
{
	float MaxHP = 1000.f;
	float CurHP = MaxHP;

	float ATTDamage = 10.f;
	float ATTSpeed	= 2000.f;

	float EXsCoolTime = 1.f;

	bool IsGroggy = false;
	bool IsDead	  = false;
};

template <typename T> class CRoRStateMachine;

class CBossScript : public CScript
{
private:
	CRoRStateMachine<CBossScript>* m_FSM;
	tBossStatus					   m_BossStatus;
	class CGameObject*			   m_Target;

	float m_AttDuration;
	float m_EXsDuration;

	bool m_ActiveAttack;
	bool m_ActiveEXs;

	bool m_ArrMissile[8]; // 미사일 발사여부
	bool m_ArrShield[2]; // 쉴드 전개 여부

	// @@디버그용
	int m_EXsType; // 현재 선택된 EX 타입

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
	void CheckVital();

	void FireMegaFist();
	void FireBossMissile(int _idx);
	void ActiveInnerShield();
	void ActiveOutsideShield();

	CRoRStateMachine<CBossScript>* GetBossFSM() { return m_FSM; }

	CGameObject* GetTarget() { return m_Target; }
	void		 SetTarget(CGameObject* _Target) { m_Target = _Target; }

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CBossScript);
	CBossScript();
	CBossScript(const CBossScript& _Origin);
	~CBossScript();

private:
	void InitStateMachine();
	void InitScriptParamUI();
};
