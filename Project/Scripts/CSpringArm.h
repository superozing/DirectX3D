#pragma once
#include <Engine\CScript.h>

class CSpringArm : public CScript
{
private:
	class CCamera* m_pTargetCam;
	Vec3		   m_vDir;
	Vec3		   m_vOffset;
	float		   m_fDistance;
	float		   m_fCamSpeed;
	float		   m_fSpeed;

	bool m_bActive;
	bool m_bType; // false : Lerp, true : Smooth

public:
	void SetTargetCamera(CCamera* _cam);

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CSpringArm);
	CSpringArm();
	~CSpringArm();
};
