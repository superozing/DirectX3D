#pragma once
#include <Engine/CScript.h>

class CImageUIScript;

class CDamageFont : public CScript
{
	// 해야 할 것.
	// 1. 데미지 폰트 쉐이더 작성
	// 2. 이미지 폰트의 사이즈를 자릿수에 비례해서 계속 조절
private:
	CImageUIScript* m_pImageUIScript;

	int				m_Damage;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetDamage(int _Damage) { m_Damage = _Damage; }

public:
	CLONE(CDamageFont);
	CDamageFont();
	~CDamageFont();
};