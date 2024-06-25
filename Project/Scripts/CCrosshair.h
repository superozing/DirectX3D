#pragma once
#include <Engine\CScript.h>

class CImageUIScript;

class CCrosshair : public CScript
{
private:
	// 상, 하, 좌, 우 조준선 이미지
	CImageUIScript* m_pCrossHair[4];

	// 조준선이 벌어지는 비율
	float m_fSpreadRatio;

public:
	virtual void begin() override;
	virtual void tick() override;

	void Fire() { m_fSpreadRatio = RoRMath::ClampFloat(m_fSpreadRatio + 0.3f, 0.f, 1.f); }

public:
	CLONE(CCrosshair);
	CCrosshair();
	~CCrosshair();
};
