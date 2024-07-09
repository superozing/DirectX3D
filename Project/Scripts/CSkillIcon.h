#pragma once
#include <Engine\CScript.h>

class CImageUIScript;

class CSkillIcon : public CScript
{
private:
	// ui
	CImageUIScript* m_pSkillIconUI;

	// texture
	Ptr<CTexture>	m_pSkillIconTex;

	// 스킬 사용 가능 여부
	bool			m_SkillUsable;

	// 스킬 쿨타임
	float			m_SkillCooldown;

	// 현재 쿨타임
	float			m_CurSkillCooldown;


public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetSkillIconTex(Ptr<CTexture> _SkillIconTex);

	void SetMaxCooldown(float _SkillCooldown)
	{ 
		m_SkillCooldown = RoRMath::ClampFloat(_SkillCooldown, 0);
	}
	void SetCurCooldown(float _CurSkillCooldown)
	{ 
		m_CurSkillCooldown = RoRMath::ClampFloat(_CurSkillCooldown, 0);
	}

	int GetMaxCooldown() const { return m_SkillCooldown; }
	int GetCurCooldown() const { return m_CurSkillCooldown; }

public:
	// 디버그용 함수
	void UseSkill();

private:
	virtual void MakeChildObjects();

public:
	CLONE(CSkillIcon);
	CSkillIcon();
	~CSkillIcon();
};
