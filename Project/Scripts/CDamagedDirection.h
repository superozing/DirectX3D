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

	Vec3 m_EnemyPos;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	void SetEnemyPos(Vec3 _EnemyPos) { m_EnemyPos = _EnemyPos; }
	void SetMaxDamageRadius(float _Angle) { m_fMaxDamageRadius = RoRMath::ClampFloat(_Angle, 0.f, XM_2PI); }
	void SetDamageRatio(float _Ratio) { m_DamageRadiusRatio = RoRMath::ClampFloat(_Ratio, 0.f, 1.f); }
	void SetAlpha(float _Alpha) { m_Alpha = RoRMath::ClampFloat(_Alpha, 0.f, 1.f); }

	float GetAlpha() const { return m_Alpha; }

private:
	void SetParentPanelUI();

public:
	CLONE(CDamagedDirection);
	CDamagedDirection();
	~CDamagedDirection();
};
