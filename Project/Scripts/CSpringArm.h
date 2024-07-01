#pragma once
#include <Engine\CScript.h>

class CSpringArm : public CScript
{
private:
	/// @brief 스프링 암에 매달리는 카메라
	CGameObject* m_pTargetCam;

	/// @brief 스프링 암이 추적하는 위치
	Vec3 m_vTargetPos;
	/// @brief 스프링 암이 향하는 각도
	Vec3 m_vDir;
	/// @brief 부모 객체의 포지션에서 시작지점을 변경하는 오프셋
	Vec3 m_vOffset;
	/// @brief 카메라가 바라보는 지점을 시작지점으로부터 돌려주는 오프셋
	// Vec3 m_vDirOffset;
	/// @brief 스프링 암의 길이
	float m_fDistance;

	/// @brief 카메라의 위치를 이동시키는 속도입니다.
	float m_fCamSpeed;
	/// @brief 카메라의 물체를 바라보는 속도 입니다.
	float m_fCamRotateSpeed;

	/// @brief 활성화 여부
	bool m_bActive;
	/// @brief false : Lerp, true : Smooth
	bool m_bType;

public:
	/// @brief 스프링 암에 붙는 카메라를 세팅하는 함수입니다.
	void SetTargetCamera(CCamera* _cam);
	/// @brief 스프링 암이 시작하는 지점부터 캠으로 향하는 방향을 세팅하는 함수입니다.
	void SetDir(Vec3 _dir) { m_vDir = _dir; }
	/// @brief 스프링 암이 시작되는 지점을 지정하는 함수입니다.
	void SetOffsetPos(Vec3 _vOffset) { m_vOffset = _vOffset; }
	/// @brief 스프링 암에 붙어있는 카메라가 바라보는 지점을 돌려주는 오프셋을 지정하는 함수입니다.
	// void SetOffsetDir(Vec3 _vOffset) { m_vDirOffset = _vOffset; }
	/// @brief 스프링 암의 길이를 지정하는 함수입니다.
	void SetDistance(float _dist) { m_fDistance = _dist; }

	/// @brief 캠이 따라붙는 속도를 설정하는 함수입니다.
	void SetCamSpeed(float _speed) { m_fCamSpeed = _speed; }
	/// @brief 캠이 시작지점으로 회전하는 속도를 설정하는 함수입니다.
	void SetCamRotSpeed(float _speed) { m_fCamRotateSpeed = _speed; }

	/// @brief 스프링 암의 활성화 여부를 결정하는 함수입니다.
	void Activate(bool _active) { m_bActive = _active; }
	/// @brief 카메라의 이동을 Smooth로 할지, Lerp로 할지 결정하는 함수입니다.
	/// @param _smooth true : Smooth, false : Lerp
	void SetMoveType(bool _type) { m_bType = _type; }

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CSpringArm);
	CSpringArm();
	~CSpringArm();
};
