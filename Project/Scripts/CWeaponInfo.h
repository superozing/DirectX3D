#pragma once
#include <Engine\CScript.h>

class CImageUIScript;

class CWeaponInfo : public CScript
{
private:
	CImageUIScript* m_pWeaponUI;

	int				m_MaxAmmo;
	int				m_CurAmmo;

	tFontInfo		m_AmmoFontInfo;
	Vec2			m_AmmoFontOffset;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void Fire();
	void Reload() { m_CurAmmo = m_MaxAmmo; }

	void SetMaxAmmo(int _MaxAmmo) { m_MaxAmmo = _MaxAmmo; }
	void SetCurAmmo(int _CurAmmo) { m_CurAmmo = _CurAmmo; }

	int GetMaxAmmo() const { return m_MaxAmmo; }
	int GetCurAmmo() const { return m_CurAmmo; }

public:
	CLONE(CWeaponInfo);
	CWeaponInfo();
	~CWeaponInfo();
};