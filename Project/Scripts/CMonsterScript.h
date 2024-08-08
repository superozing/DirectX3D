#pragma once
#include <Engine/CScript.h>
#include <Engine\CPhysXMgr.h>

enum class MONSTER_STATE
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

struct tMonsterStatus
{
	float MaxHP = 1000.f;
	float CurHP = MaxHP;

	float ATTDamage = 10.f;
	float ATTSpeed	= 2000.f;
	float ATTCoolTime = 1.f;

	float DetactRange = 1000.f;

	bool IsGroggy = false;
	bool IsDead	  = false;
};

template <typename T> class CRoRStateMachine;


class CMonsterScript : public CScript
{
private:
	CRoRStateMachine<CMonsterScript>* m_FSM;
	tMonsterStatus					  m_BossStatus;

	class CGameObject*				  m_Target;
	tRoRHitInfo						  m_hitInfo;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

public:
	CLONE(CMonsterScript);
	CMonsterScript();
	CMonsterScript(const CMonsterScript& _Origin);
	~CMonsterScript();
};
