#pragma once
#include <Engine\CScript.h>

class CSpringArm : public CScript
{
private:
	class CCamera* m_pTargetCam;
	float		   m_fDistance;
	float		   m_fSpeed;
	CGameObject*   m_target;

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
