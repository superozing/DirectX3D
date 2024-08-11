#pragma once
#include "CMonsterScript.h"
#include <Engine/CPhysXMgr.h>

enum class DROIDAR_STATE
{
	Appere,
	EXs,
	NormalIdle,
	NormalReload,

	NormalAttackStart,
	NormalAttackIng,
	NormalAttackDelay,
	NormalAttackEnd,
	
	MoveIng,
	MoveEnd,

	VitalPanic,
	VitalDeath,

	END,
};

/*
struct MonsterStatus
{
	int MaxHealth = 100.f;
	int CurHealth = MaxHealth;

	float Damage	= 5.f;
	float Defensive = 5.f;

	float MoveSpeed		  = 500.f;
	float AttackMoveSpeed = MoveSpeed * 0.2f;
	float RotateSpeed	  = 20.f;

	float AvoidPercent	  = 10.f;
	float CriticalPercent = 0.f;
	float CriticalDamage  = 1.2f;

	bool IsDead = false;

	bool Invincibility = false;
};
*/

template <typename T> class CRoRStateMachine;

class CDroidAR : public CMonsterScript
{
private:
	CRoRStateMachine<CDroidAR>*		m_FSM;
	MonsterStatus					m_BossStatus;
	class CGameObject*				m_Target;
	tRoRHitInfo						m_hitInfo;


#pragma region About State

public:
	void AppereBegin();
	int  AppereUpdate();
	void AppereEnd();
	
	void EXsBegin();
	int  EXsUpdate();
	void EXsEnd();
	
	void NormalIdleBegin();
	int  NormalIdleUpdate();
	void NormalIdleEnd();
	
	void NormalReloadBegin();
	int  NormalReloadUpdate();
	void NormalReloadEnd();
	
	void NormalAttackStartBegin();
	int  NormalAttackStartUpdate();
	void NormalAttackStartEnd();
	
	void NormalAttackIngBegin();
	int	 NormalAttackIngUpdate();
	void NormalAttackIngEnd();
	
	void NormalAttackDelayBegin();
	int  NormalAttackDelayUpdate();
	void NormalAttackDelayEnd();
	
	void NormalAttackEndBegin();
	int  NormalAttackEndUpdate();
	void NormalAttackEndEnd();
	
	void MoveIngBegin();
	int  MoveIngUpdate();
	void MoveIngEnd();
	
	void MoveEndBegin();
	int  MoveEndUpdate();
	void MoveEndEnd();
	
	void VitalPanicBegin();
	int  VitalPanicUpdate();
	void VitalPanicEnd();
	
	void VitalDeathBegin();
	int  VitalDeathUpdate();
	void VitalDeathEnd();

#pragma endregion


public:
	virtual void begin() override;
	virtual void tick() override;

	void LoadAsset();

	void CheckDuration();

	void CheckVital();

public:
	CLONE(CDroidAR);
	CDroidAR();
	CDroidAR(const CDroidAR& _Origin);
	~CDroidAR();

private:
	void InitStateMachine();
	void InitScriptParamUI();
};
