#pragma once
#include "CMonsterScript.h"
#include <Engine\CPhysXMgr.h>

#define AttackTimer 10.f
#define AttackDistance 2500.f

enum class TURRET_STATE
{
	Appear,
	NormalIdle,
	Reload,
	AttackStart,
	AttackDelay,
	Attack,
	AttackEnd,
	VitalRetreat,
	VitalPanic,
	VitalDeath,
	END,
};

template <typename T> class CRoRStateMachine;
class CGameObject;

class CTurret : public CMonsterScript
{
private:
	CRoRStateMachine<CTurret>* m_FSM;
	CGameObject*			   m_Target;
	tRoRHitInfo				   m_hitInfo;

	float fDetectDistance;
	float fAttackTimer;

private:
	void		 SetTurretInfo();
	TURRET_STATE m_TurretState;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

#pragma region About State

	void AppearBegin();
	int	 AppearUpdate();
	void AppearEnd();

	void NormalIdleBegin();
	int	 NormalIdleUpdate();
	void NormalIdleEnd();

	void ReloadBegin();
	int	 ReloadUpdate();
	void ReloadEnd();

	void AttackStartBegin();
	int	 AttackStartUpdate();
	void AttackStartEnd();

	void AttackDelayBegin();
	int	 AttackDelayUpdate();
	void AttackDelayEnd();

	void AttackBegin();
	int	 AttackUpdate();
	void AttackEnd();

	void AttackEndBegin();
	int	 AttackEndUpdate();
	void AttackEndEnd();

	void VitalRetreatBegin();
	int	 VitalRetreatUpdate();
	void VitalRetreatEnd();

	void VitalPanicBegin();
	int	 VitalPanicUpdate();
	void VitalPanicEnd();

	void VitalDeathBegin();
	int	 VitalDeathUpdate();
	void VitalDeathEnd();

public:
#pragma region StatusFunc

	void InitStateMachine();

	void SwitchNormal();
	void SwitchPanic();
	void SwitchDead();

	int GetCurState();

	void RotateIdle();

	void  CheckAttackable();
	float CalTargetDistance();
	void  CheckVital();

public:
	CLONE(CTurret);
	CTurret();
	CTurret(const CTurret& _Origin);
	~CTurret();
};
