#pragma once
#include <Engine\CScript.h>

class CImageUIScript;
class CPanelUIScript;

class CCrosshair : public CScript
{
private:
	// 부모 패널UI
	CPanelUIScript* m_pPanelUI;

	// 상, 하, 좌, 우 조준선 이미지
	CImageUIScript* m_pCrossHair[4];

	// 조준선이 최대 벌어지는 정도
	float m_fMaxSpread;
	// 조준선이 최소 벌어지는 정도
	float m_fMinSpread;

	// 조준선이 벌어지는 비율
	float m_fSpreadRatio;

public:
	virtual void begin() override;
	virtual void tick() override;

	void SetMaxSpread(float _fMaxSpread) { m_fMaxSpread = RoRMath::ClampFloat(_fMaxSpread, 0.f); }
	void SetMinSpread(float _fMinSpread) { m_fMinSpread = RoRMath::ClampFloat(_fMinSpread, 0.f); }
	void SetSpreadRatio(float _fSpreadRatio) { m_fSpreadRatio = RoRMath::ClampFloat(_fSpreadRatio, 0.f, 1.f); }

	float GetMaxSpread() const { return m_fMaxSpread; }
	float GetMinSpread() const { return m_fMinSpread; }
	float GetSpreadRatio() const { return m_fSpreadRatio; }

private:
	void SetParentPanelUI();

public:
	CLONE(CCrosshair);
	CCrosshair();
	~CCrosshair();
};
