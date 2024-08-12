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
	// 세 발 동안 탄창을 모두 소모한 이후 장전 모션을 가지면 될 것 같아요.
	// 플레이어와의 거리가 일정 거리 이상 멀어질 경우,
	// 플레이어 위치로 AttackRange 안에 들어올 때 까지 이동 시켜주어요.
private:
	CRoRStateMachine<CDroidAR>*		m_FSM;
	MonsterStatus					m_MonsterStatus;

	class CShootingSystem_DroidAR* m_ShootingSystem;

	// 타겟 오브젝트 (플레이어)
	class CGameObject*				m_Target;
	tRoRHitInfo						m_hitInfo;

	class CMuzzleFlash_DroidAR*		m_MuzzleFlash;

	int								m_AttackCount;
	int								m_MaxAttackCount = 3;

	bool							m_IsActiveAttack;
	float							m_AttCooldown;

	Vec3							m_LookDirection;

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

	void CheckAttackCooldown();

	void CheckVital();

	bool CheckTargetInRange();

public:
	CLONE(CDroidAR);
	CDroidAR();
	CDroidAR(const CDroidAR& _Origin);
	~CDroidAR();

private:
	void InitStateMachine();
	void InitScriptParamUI();
	void RotationDirectionToPlayer();
};
