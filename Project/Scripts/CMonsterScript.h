#pragma once
#include <Engine/CScript.h>

class CMonsterScript : public CScript
{
private:
	// 현재 체력
	int m_CurHP;
	
	// 최대 체력
	int m_MaxHP;

	// 사망한 몬스터 오브젝트인가?
	bool m_IsDeadMonster;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	void SetCurHP(int _HP) { m_CurHP = RoRMath::ClampInt(_HP, 0); }
	void SetMaxHP(int _HP) { m_MaxHP = RoRMath::ClampInt(_HP, 0); }
	
	int GetCurHP() const { return m_CurHP; }
	int GetMaxHP() const { return m_MaxHP; }
	
	void TakeDamage(int _Dmg);

	bool IsDeadMonster() const { return m_IsDeadMonster; }

private:
	void DbgTakeDamage();

public:
	CLONE(CMonsterScript);
	CMonsterScript();
	CMonsterScript(const CMonsterScript& _Origin);
	~CMonsterScript();
};
