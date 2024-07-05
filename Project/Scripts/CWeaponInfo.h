#pragma once
#include <Engine\CScript.h>

class CImageUIScript;
class CPanelUIScript;
class CAmmoInfo;

class CWeaponInfo : public CScript
{
private:
	// 기반 패널 UI
	CPanelUIScript* m_pPanelUI;

	// 무기 이미지
	CImageUIScript* m_pWeaponUI;

	// 탄창 상태
	CAmmoInfo* m_pAmmoProgressBar;

	// 최대 탄 개수
	int m_MaxAmmo;

	// 현재 탄 개수
	int m_CurAmmo;

	// 최대 탄창 상태 폰트
	tFontInfo m_MaxAmmoFont;
	Vec2	  m_MaxAmmoFontOffset;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void Fire();
	void Reload() { m_CurAmmo = m_MaxAmmo; }

	void SetMaxAmmo(int _MaxAmmo) { m_MaxAmmo = RoRMath::ClampInt(_MaxAmmo, 0); }
	void SetCurAmmo(int _CurAmmo) { m_CurAmmo = RoRMath::ClampInt(_CurAmmo, 0); }

	int GetMaxAmmo() const { return m_MaxAmmo; }
	int GetCurAmmo() const { return m_CurAmmo; }

	// 나중에 총 정보 구조체를 만들어서 넣어주면 좋을 것 같다.

private:
	void SetParentPanelUI();

public:
	CLONE(CWeaponInfo);
	CWeaponInfo();
	~CWeaponInfo();
};