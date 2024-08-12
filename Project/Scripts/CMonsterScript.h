#pragma once
#include <Engine/CScript.h>

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

class CMonsterScript : public CScript
{
protected:
	MonsterStatus m_tStatus;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	void SetCurHP(int _HP) { m_tStatus.CurHealth = RoRMath::ClampInt(_HP, 0); }
	void SetMaxHP(int _HP) { m_tStatus.MaxHealth = RoRMath::ClampInt(_HP, 0); }

	int GetCurHP() const { return m_tStatus.CurHealth; }
	int GetMaxHP() const { return m_tStatus.MaxHealth; }

	void TakeDamage(int _Dmg);

	bool IsDeadMonster() const { return m_tStatus.IsDead; }

	float GetDamageVal() const { return m_tStatus.Damage; }

private:
	void DbgTakeDamage();

public:
	CLONE(CMonsterScript);
	CMonsterScript();
	CMonsterScript(UINT _type);
	CMonsterScript(const CMonsterScript& _Origin);
	~CMonsterScript();
};
