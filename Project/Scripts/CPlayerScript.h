#pragma once

#include <Engine/CScript.h>

enum class PLAYER_STATE
{
	NORMAL,
	ATTACK,
	COVER,
	COVERATTACK,
	COVERKNEEATTACK,
	DEATH,
	CEREMONY,
	END,
};

struct PlayerStatus
{
	int MaxHealth  = 100;
	int curHealth  = MaxHealth;
	int MaxStamina = 100;
	int curStamina = MaxStamina;
	int Damage	   = 5;
	int Defensive  = 5;

	float MoveSpeed		  = 500.f;
	float AttackMoveSpeed = MoveSpeed * 0.02f;
	float RotateSpeed	  = 20.f;

	float AvoidPercent	  = 10.f;
	float CriticalPercent = 0.f;
};

template <typename T> class CRoRStateMachine;

class CPlayerScript : public CScript
{
private:
	CRoRStateMachine<CPlayerScript>* m_FSM;

	PlayerStatus m_tStatus;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
	virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
	virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;
	CLONE(CPlayerScript);

#pragma region About State
	void NormalBegin();
	int	 NormalUpdate();
	void NormalEnd();
	void AttackBegin();
	int	 AttackUpdate();
	void AttackEnd();
	void CoverBegin();
	int	 CoverUpdate();
	void CoverEnd();
	void CoverAttackBegin();
	int	 CoverAttackUpdate();
	void CoverAttackEnd();
	void CoverKneeAttackBegin();
	int	 CoverKneeAttackUpdate();
	void CoverKneeAttackEnd();

	void DeathBegin();
	int	 DeathUpdate();
	void DeathEnd();
	void CeremonyBegin();
	int	 CeremonyUpdate();
	void CeremonyEnd();

#pragma endregion

public:
	CPlayerScript();
	~CPlayerScript();
};
