#pragma once
#include <Engine\CScript.h>
class CCameraEffect : public CScript
{
private:
	static Vec3	 m_vInitialPos;
	static Vec3	 m_vInitialRotation;
	static float m_fInitialFOV;
	static float m_fInitialScale;

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
