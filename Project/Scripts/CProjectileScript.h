#pragma once
#include <Engine\CScript.h>

class CProjectileScript : public CScript
{
protected:
	Vec3 m_Pos; // 투사체 위치

	float m_InitialSpeed; // 투사체 초기 속력
	float m_MaxSpeed;	  // 투사체 최대 속력

	float m_LifeSpan; // 투사체 생존 시간
	float m_Damage;	  // 투사체 데미지

	bool m_Explode; // 투사체 충돌 시 폭발 여부
	bool m_IsAlive; // 투사체 활성 여부

public:
	virtual void InitProjectileInfo(Vec3 _Pos, float _InitSpeed, float _MaxSpeed, float _LifeSpan, float _Damage,
									bool _Explode, bool _Alive);

	virtual void OnHit(); // 다른 충돌체와 충돌했을 때 호출되는 함수

#pragma region GET_SET FUNC

public:
	Vec3 GetPos() { return m_Pos; }
	void SetPos(Vec3 _vPos) { m_Pos = _vPos; }

	float GetInitialSpeed() { return m_InitialSpeed; }
	float GetMaxSpeed() { return m_MaxSpeed; }
	void  SetInitialSpeed(float _speed) { m_InitialSpeed = _speed; }
	void  SetMaxSpeed(float _speed) { m_MaxSpeed = _speed; }

	float GetLifeSpan() { return m_LifeSpan; }
	float GetDamage() { return m_Damage; }
	void  SetLifeSpan(float _life) { m_LifeSpan = _life; }
	void  SetDamage(float _damage) { m_Damage = _damage; }

	bool IsAlive() { return m_IsAlive; }
	void SetAlive(bool _alive) { m_IsAlive = _alive; }

	bool IsExplode() { return m_Explode; }
	void SetExplode(bool _explode) { m_Explode = _explode; }

#pragma endregion

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CProjectileScript);
	CProjectileScript(UINT _ScriptType);
	~CProjectileScript();

protected:
	virtual void InitProjectileScriptParamUI();
};
