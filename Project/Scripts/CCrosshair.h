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
	CImageUIScript* m_pCrossHair;

	// 조준선이 벌어지는 비율
	float m_fSpreadRatio;

	// 조준선 막대기 넓이
	float m_fBarThikness;

	// 조준선 막대기 길이
	float m_fLength;

	// 조준선 색상
	Vec4 m_CrosshairColor;

public:
	virtual void begin() override;
	virtual void tick() override;

	void SetSpreadRatio(float _fSpreadRatio) { m_fSpreadRatio = RoRMath::ClampFloat(_fSpreadRatio, 0.f, 1.f); }
	void SetCrosshairColor(Vec4 _CrosshairColor) { m_CrosshairColor = _CrosshairColor; }

	float GetSpreadRatio() const { return m_fSpreadRatio; }
	Vec4  GetCrosshairColor() const { return m_CrosshairColor; }

private:
	void SetParentPanelUI();

public:
	CLONE(CCrosshair);
	CCrosshair();
	~CCrosshair();
};
