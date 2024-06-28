#pragma once
#include <Engine\CScript.h>

class CSpringArm : public CScript
{
private:
	CGameObject* m_pTargetCam;
	Vec3		 m_vDir;
	Vec3		 m_vOffset;
	float		 m_fDistance;

	/// @brief 카메라의 위치를 이동시키는 속도입니다.
	float m_fCamSpeed;
	/// @brief 카메라의 물체를 바라보는 속도 입니다.
	float m_fCamRotateSpeed;
	/// @brief 카메라 암의 회전 속도입니다.
	float m_fArmRotateSpeed;

	/// @brief 활성화 여부
	bool m_bActive;
	/// @brief false : Lerp, true : Smooth
	bool m_bType;

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
