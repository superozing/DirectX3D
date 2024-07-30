#pragma once
#include <Engine/CScript.h>

class CImageUIScript;
class CPanelUIScript;

class CDamagedDirection : public CScript
{
private:
	// 방향 이미지
	CImageUIScript* m_pImageUI;

	// 최대 데미지를 입을 경우의 반지름
	float m_fMaxDamageRadius;

	float m_DamageRadiusRatio;

	float m_Alpha;

	Vec3 m_EnemyPos;
	Vec3 m_PlayerPos;
	Vec3 m_PlayerDir;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	void SetEnemyPos(Vec3 _EnemyPos);
	void SetMaxDamageRadius(float _Angle) { m_fMaxDamageRadius = RoRMath::ClampFloat(_Angle, 0.f, XM_2PI); }
	void SetDamageRatio(float _Ratio) { m_DamageRadiusRatio = RoRMath::ClampFloat(_Ratio, 0.f, 1.f); }
	void SetAlpha(float _Alpha) { m_Alpha = RoRMath::ClampFloat(_Alpha, 0.f, 1.f); }

	float GetAlpha() const { return m_Alpha; }

public:
	CLONE(CDamagedDirection);
	CDamagedDirection();
	CDamagedDirection(const CDamagedDirection& _Origin);
	~CDamagedDirection();
};
