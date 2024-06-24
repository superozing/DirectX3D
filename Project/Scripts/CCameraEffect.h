#pragma once
#include <Engine\CScript.h>
class CCameraEffect : public CScript
{
private:
	Vec3  m_vInitialPos;
	Vec3  m_vInitialRotation;
	float m_fInitialFOV;
	float m_fInitialScale;

public:
	void RegistInitial();
	void SendToInitial();

public:
	virtual void tick() override;
	virtual void begin() override;

public:
	CLONE(CCameraEffect);
	CCameraEffect();
	~CCameraEffect();
};
