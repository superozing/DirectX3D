#pragma once
#include <Engine/CScript.h>

class CImageUIScript;
class CPanelUIScript;

class CDamagedDirection : public CScript
{
private:
	// 기반 패널UI
	CPanelUIScript* m_pPanelUI;

	// 방향 이미지
	CImageUIScript* m_pImageUI;

	// 최대 데미지를 입을 경우의 반지름
	float m_fMaxDamageRadius;

	float m_DamageRadiusRatio;

	float m_Alpha;

	Vec3 m_DamagedDirection;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	void SetDamagedDirection(Vec3 _DamagedDirection) { m_DamagedDirection = _DamagedDirection; }
	void SetMaxDamageRadius(float _Angle) { m_fMaxDamageRadius = _Angle; }
	void SetAlpha(float _Alpha) { m_Alpha = _Alpha; }

private:
	void SetParentPanelUI();

public:
	CLONE(CDamagedDirection);
	CDamagedDirection();
	~CDamagedDirection();
};
