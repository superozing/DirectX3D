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
	float MaxHealth	 = 100.f;
	float curHealth	 = MaxHealth;
	float MaxStamina = 100.f;
	float curStamina = MaxStamina;

	float Damage	= 5.f;
	float Defensive = 5.f;

	float MoveSpeed		  = 500.f;
	float AttackMoveSpeed = MoveSpeed * 0.04f;
	float RotateSpeed	  = 20.f;

	float AvoidPercent	  = 10.f;
	float CriticalPercent = 0.f;
	float CriticalDamage  = 1.2f;

	bool IsDead = false;
};

template <typename T> class CRoRStateMachine;

class CPlayerScript : public CScript
{
private:
	CRoRStateMachine<CPlayerScript>* m_FSM;

	PlayerStatus m_tStatus;

public:
	/// @brief 플레이어 캐릭터에 파라미터 만큼 데미지를 줍니다. 현재 체력이 0 이하로 떨어지면 Dead상태로 만듭니다.
	/// 아직은 회피율을 계산하지 않습니다.
	void Hit(float _damage)
	{
		m_tStatus.curHealth -= _damage;
		if (m_tStatus.curHealth <= 0.f)
			m_tStatus.IsDead = true;
	}
	/// @brief 플레이어 캐릭터에게 파라미터 만큼 회복을 합니다. 최대체력 캡이 보장됩니다.
	void Recursive(float _heal)
	{
		m_tStatus.curHealth += _heal;
		if (m_tStatus.curHealth > m_tStatus.MaxHealth)
		{
			m_tStatus.curHealth = m_tStatus.MaxHealth;
		}
	}
	/// @brief 현재 체력을 가져옵니다.
	float CurHealth() { return m_tStatus.curHealth; }
	/// @brief 현재 체력을 파라미터 값으로 세팅합니다. 데미지를 주거나 회복을 하기 위해서는 Hit이나 Recursive함수를
	/// 사용하는 것을 추천합니다.
	void SetCurHealth(float _absValue)
	{
		m_tStatus.curHealth = _absValue;
		if (m_tStatus.curHealth < 0.f)
			m_tStatus.curHealth = 0.f;
		m_tStatus.IsDead = true;
	}
	/// @brief 최대 체력을 파라미터만큼 추가하거나 줄이는 함수입니다. 언더캡이 보장됩니다.
	void ModifyMaxHealth(float _relValue)
	{
		m_tStatus.MaxHealth += _relValue;

		// 최대체력이 증가할 경우 현재 체력도 늘려줌
		if (_relValue >= 0.f)
			m_tStatus.curHealth += _relValue;

		// 최대 체력이 0 이하로 내려갈 경우 1로 보정
		if (m_tStatus.MaxHealth <= 0.f)
			m_tStatus.MaxHealth = 1.f;

		// 최대체력이 현재체력보다 깎일 경우 최대채력으로 갱신
		if (m_tStatus.curHealth > m_tStatus.MaxHealth)
			m_tStatus.curHealth = m_tStatus.MaxHealth;
	}

	/// @brief 현재 스태미나를 얻어옵니다.
	float CurStamina() { return m_tStatus.curStamina; }
	/// @brief 현재 스테미나를 파라미터만큼 변경합니다. 최대 스태미나 캡이 보장됩니다.
	void ModifyCurStamina(float _relValue)
	{
		m_tStatus.curStamina += _relValue;

		// 0 이하로 내려갔을 경우
		if (m_tStatus.curStamina < 0.f)
			m_tStatus.curStamina = 0.f;

		// 최대 체력보다 많아졌을 경우
		if (m_tStatus.curStamina > m_tStatus.MaxStamina)
			m_tStatus.curStamina = m_tStatus.MaxStamina;
	}
	/// @brief 현재 스테미나를 파라미터로 변경합니다.  스테미나 사용이나 회복을 위해서는 ModifyCurStamina 함수 사용을
	/// 추천합니다.
	void SetCurStamina(float _absValue)
	{
		m_tStatus.curStamina = _absValue;

		// 0 이하로 떨어졌을 경우 보정
		if (m_tStatus.curStamina < 0.f)
			m_tStatus.curStamina = 0.f;

		// 최대 스테미나보다 커졌을 경우 조정
		if (m_tStatus.curStamina > m_tStatus.MaxStamina)
			m_tStatus.curStamina = m_tStatus.MaxStamina;
	}
	/// @brief 최대 스태미나를 파라미터만큼 추가하거나 줄이는 함수입니다. 언더캡이 보장됩니다.
	void ModifyMaxStamina(float _value)
	{
		m_tStatus.MaxStamina += _value;

		// 최대 스테미나가 0이하로 내려갔을 경우 1로 보정
		if (m_tStatus.MaxStamina < 0.f)
			m_tStatus.MaxStamina = 1.f;

		// 현재 스테미나보다 최대 스테미나가 깎였을 경우 현재 스테미나 조정
		if (m_tStatus.curStamina > m_tStatus.MaxStamina)
			m_tStatus.curStamina = m_tStatus.MaxStamina;
	}

	/// @brief 현재 데미지를 반환합니다. 현재는 크리티컬 확률을 계산하지 않습니다.
	float GetDamage() { return m_tStatus.Damage; }
	/// @brief 데미지를 파라미터 만큼 올리거나 낮춥니다. 언더캡이 보장됩니다.
	void AddDamage(float _relValue)
	{
		m_tStatus.Damage += _relValue;
		if (m_tStatus.Damage < 0.f)
			m_tStatus.Damage = 0.f;
	}
	/// @brief 데미지를 파라미터로 변경합니다. 언더캡이 보장됩니다.
	void SetDamage(float _absValue)
	{
		m_tStatus.Damage = _absValue;
		if (m_tStatus.Damage < 0.f)
			m_tStatus.Damage = 0.f;
	}

	/// @brief 현재 방어도를 반환합니다.
	float GetDefensive() { return m_tStatus.Defensive; }

	/// @brief 사망 여부를 반환합니다.
	bool IsDead() { return m_tStatus.IsDead; }

public:
	virtual void begin() override;
	virtual void tick() override;

	CRoRStateMachine<CPlayerScript>* m_FSM;

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
	CPlayerScript(const CPlayerScript& _origin);
	~CPlayerScript();
};
