#pragma once

#include <Engine/CScript.h>

enum class PLAYER_STATE
{
	NormalIdle,
	NormalReload,
	NormalAttackStart,
	NormalAttackIng,
	NormalAttackDelay,
	NormalAttackEnd,
	StandIdle,
	StandReload,
	StandAttackStart,
	StandAttackIng,
	StandAttackDelay,
	StandAttackEnd,
	KneelIdle,
	KneelReload,
	KneelAttackStart,
	KneelAttackIng,
	KneelAttackDelay,
	KneelAttackEnd,
	MoveStartNormal,
	MoveStartStand,
	MoveStartKneel,
	MoveIng,
	MoveJump,
	MoveEndNormal,
	MoveEndStand,
	MoveEndKneel,
	VitalDeath,
	VitalPanic,
	VitalDying,
	VictoryStart,
	VictoryEnd,
	SkillDash,
	SkillThrow,
	SkillCallsign,
	SkillEX,
	FormationIdle,
	END,
};

/// @brief 0: 엄폐 없음, 1: 높은 엄폐, 2: 낮은 엄폐
enum class CoverType
{
	Normal,
	Stand,
	Kneel,
	End,
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
	float JumpSpeed		  = 100.f;
	float AttackMoveSpeed = MoveSpeed * 0.2f;
	float DashMaxSpeed	  = MoveSpeed * 2.f;
	float DashGroundSpeed = MoveSpeed * 1.f;
	float DashMinSpeed	  = MoveSpeed * 0.05f;
	float RotateSpeed	  = 20.f;

	float AvoidPercent	  = 10.f;
	float CriticalPercent = 0.f;
	float CriticalDamage  = 1.2f;

	bool IsDead = false;
};

template <typename T> class CRoRStateMachine;
#include "CSpringArm.h"

class CPlayerScript : public CScript
{
private:
	CRoRStateMachine<CPlayerScript>* m_FSM;

	PlayerStatus m_tStatus;

	/// @brief 현재 엄폐 타입입니다.
	CoverType m_iCorverType;

	class CSpringArm* m_pSpringArm;

	map<PLAYER_STATE, SpringArmInfo> m_mSpringInfos;

public:
#pragma region StatusFunc
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
#pragma endregion

	void	  SetCoverType(CoverType _type) { m_iCorverType = _type; }
	CoverType GetCoverType() { return m_iCorverType; }

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

	void StandIdleBegin();
	int	 StandIdleUpdate();
	void StandIdleEnd();
	void StandReloadBegin();
	int	 StandReloadUpdate();
	void StandReloadEnd();
	void StandAttackStartBegin();
	int	 StandAttackStartUpdate();
	void StandAttackStartEnd();
	void StandAttackIngBegin();
	int	 StandAttackIngUpdate();
	void StandAttackIngEnd();
	void StandAttackDelayBegin();
	int	 StandAttackDelayUpdate();
	void StandAttackDelayEnd();
	void StandAttackEndBegin();
	int	 StandAttackEndUpdate();
	void StandAttackEndEnd();

	void KneelIdleBegin();
	int	 KneelIdleUpdate();
	void KneelIdleEnd();
	void KneelReloadBegin();
	int	 KneelReloadUpdate();
	void KneelReloadEnd();
	void KneelAttackStartBegin();
	int	 KneelAttackStartUpdate();
	void KneelAttackStartEnd();
	void KneelAttackIngBegin();
	int	 KneelAttackIngUpdate();
	void KneelAttackIngEnd();
	void KneelAttackDelayBegin();
	int	 KneelAttackDelayUpdate();
	void KneelAttackDelayEnd();
	void KneelAttackEndBegin();
	int	 KneelAttackEndUpdate();
	void KneelAttackEndEnd();

	void MoveStartNormalBegin();
	int	 MoveStartNormalUpdate();
	void MoveStartNormalEnd();
	void MoveStartStandBegin();
	int	 MoveStartStandUpdate();
	void MoveStartStandEnd();
	void MoveStartKneelBegin();
	int	 MoveStartKneelUpdate();
	void MoveStartKneelEnd();
	void MoveEndNormalBegin();
	int	 MoveEndNormalUpdate();
	void MoveEndNormalEnd();
	void MoveEndStandBegin();
	int	 MoveEndStandUpdate();
	void MoveEndStandEnd();
	void MoveEndKneelBegin();
	int	 MoveEndKneelUpdate();
	void MoveEndKneelEnd();
	void MoveIngBegin();
	int	 MoveIngUpdate();
	void MoveIngEnd();
	void MoveJumpBegin();
	int	 MoveJumpUpdate();
	void MoveJumpEnd();

	void VitalDeathBegin();
	int	 VitalDeathUpdate();
	void VitalDeathEnd();
	void VitalPanicBegin();
	int	 VitalPanicUpdate();
	void VitalPanicEnd();
	void VitalDyingBegin();
	int	 VitalDyingUpdate();
	void VitalDyingEnd();

	void VictoryStartBegin();
	int	 VictoryStartUpdate();
	void VictoryStartEnd();
	void VictoryEndBegin();
	int	 VictoryEndUpdate();
	void VictoryEndEnd();

	void SkillDashBegin();
	int	 SkillDashUpdate();
	void SkillDashEnd();
	void SkillThrowBegin();
	int	 SkillThrowUpdate();
	void SkillThrowEnd();
	void SkillCallsignBegin();
	int	 SkillCallsignUpdate();
	void SkillCallsignEnd();
	void SkillEXBegin();
	int	 SkillEXUpdate();
	void SkillEXEnd();

	void FormationIdleBegin();
	int	 FormationIdleUpdate();
	void FormationIdleEnd();

private:
	float m_fJumpY;

#pragma endregion

public:
	CPlayerScript();
	CPlayerScript(const CPlayerScript& _origin);
	~CPlayerScript();

private:
	/// @brief 스크립트 파람 UI 들을 초기화하는 함수입니다.
	void InitScriptParamUI();
	/// @brief 스테이트 머신의 함수들을 초기화하는 함수입니다.
	void InitStateMachine();
	/// @brief 스프링 암의 상태를 초기화하는 함수입니다.
	void InitSpringArmSetting();

	/// @brief 카메라 움직임을 제어하는 함수입니다.
	void CameraMove();
	/// @brief 일반 움직임을 다루는 함수입니다.
	void NormalMove();
	/// @brief 커버타입을 구분지어 Idle상태를 돌려주는 함수입니다.
	int SwitchToCoverTypeIdle();
	/// @brief 커버타입을 구분지어 MoveEnd상태를 돌려주는 함수입니다.
	int SwitchToCoverTypeMoveEnd();
	/// @brief 조건에 따라 일반 움직임으로 변경해주는 함수입니다.
	void ChangeToMove();
	/// @brief 엄폐상태에서 상태 조건에 따라 노말 상태로 돌아오는 함수입니다.
	void ChangeToNormal();
	/// @brief 상태 조건에 따라 victory상태로 변경하는 함수입니다.
	void ChangeToVictory();
	/// @brief 상태 조건에 따라 Dash 상태로 변경하는 함수입니다.
	void ChangeToDash();
	/// @brief 조건에 따라 일반 공격으로 변경해주는 함수입니다.
	void NormalAttack();

	// Test 함수
private:
	/// @brief 커버 타입 판정 하기 전까지 필요한 함수
	void SwitchCoverType();
};
