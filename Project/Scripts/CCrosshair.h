﻿#pragma once
#include <Engine\CScript.h>

class CImageUIScript;
class CPanelUIScript;

class CCrosshair : public CScript
{
private:
	// 조준선이 벌어지는 비율
	float m_fSpreadRatio;

	// 조준선 막대기 넓이
	float m_fBarThikness;

	// 조준선 막대기 길이
	float m_fLength;

	// 조준선 색상
	Vec4 m_CrosshairColor;

	bool m_bShootAvailable;

	CGameObject*		 m_pPlayer;
	class CPlayerScript* m_pPlayerScript;

	class CDamagedDirectionMgr* m_pDamagedDirectionMgr;

	class CBulletMarkSpawner* m_pBulletMarkDecalSpawner;

public:
	void SetShootAvailable(bool _avail);
	bool IsShootingAvailable() { return m_bShootAvailable; }
	void CurAimLayer(LAYER _layer);

	void ShootBulletRay();

public:
	virtual void begin() override;
	virtual void tick() override;

	void SetSpreadRatio(float _fSpreadRatio) { m_fSpreadRatio = RoRMath::ClampFloat(_fSpreadRatio, 0.f, 1.f); }
	void SetCrosshairColor(Vec4 _CrosshairColor) { m_CrosshairColor = _CrosshairColor; }

	float GetSpreadRatio() const { return m_fSpreadRatio; }
	Vec4  GetCrosshairColor() const { return m_CrosshairColor; }

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CCrosshair);
	CCrosshair();
	~CCrosshair();
};
