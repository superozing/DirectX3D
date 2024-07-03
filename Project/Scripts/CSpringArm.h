#pragma once
#include <Engine\CScript.h>

struct SpringArmInfo
{
	/// @brief 스프링 암이 향하는 각도
	Vec3 vDir = Vec3(0.f, 1.f, 1.f);
	/// @brief 부모 객체의 포지션에서 시작지점을 변경하는 오프셋
	Vec3 vOffset;
	/// @brief 카메라가 바라보는 지점을 시작지점으로부터 돌려주는 오프셋
	Vec3 vDirOffset;
	/// @brief 스프링 암의 길이
	float fDistance = 500.f;
	/// @brief 카메라의 위치를 이동시키는 속도입니다.
	float fCamSpeed = 20.f;
	/// @brief 카메라의 물체를 바라보는 속도 입니다.
	float fCamRotSpeed = 20.f;
	/// @brief false : Lerp, true : Smooth
	bool Type = true;
};

class CSpringArm : public CScript
{
private:
	/// @brief 스프링 암에 매달리는 카메라
	CGameObject* m_pTargetCam;
	/// @brief 활성화 여부
	bool m_bActive = true;

	SpringArmInfo m_tInfo;

public:
	/// @brief 스프링 암에 붙는 카메라를 세팅하는 함수입니다.
	void SetTargetCamera(CCamera* _cam);
	/// @brief 스프링 암이 시작하는 지점부터 캠으로 향하는 방향을 세팅하는 함수입니다.
	void SetDir(Vec3 _dir) { m_tInfo.vDir = _dir; }
	/// @brief 스프링 암이 시작되는 지점을 지정하는 함수입니다.
	void SetOffsetPos(Vec3 _vOffset) { m_tInfo.vOffset = _vOffset; }
	/// @brief 스프링 암에 붙어있는 카메라가 바라보는 지점을 돌려주는 오프셋을 지정하는 함수입니다.
	void SetOffsetDir(Vec3 _vOffset) { m_tInfo.vDirOffset = _vOffset; }
	/// @brief 스프링 암의 길이를 지정하는 함수입니다.
	void SetDistance(float _dist) { m_tInfo.fDistance = _dist; }

	/// @brief 캠이 따라붙는 속도를 설정하는 함수입니다.
	void SetCamSpeed(float _speed) { m_tInfo.fCamSpeed = _speed; }
	/// @brief 캠이 시작지점으로 회전하는 속도를 설정하는 함수입니다.
	void SetCamRotSpeed(float _speed) { m_tInfo.fCamRotSpeed = _speed; }

	/// @brief 스프링 암의 활성화 여부를 결정하는 함수입니다.
	void Activate(bool _active) { m_bActive = _active; }
	/// @brief 카메라의 이동을 Smooth로 할지, Lerp로 할지 결정하는 함수입니다.
	/// @param _smooth true : Smooth, false : Lerp
	void SetMoveType(bool _type) { m_tInfo.Type = _type; }

	SpringArmInfo GetInfo() { return m_tInfo; }
	void		  SetInfo(SpringArmInfo _info) { m_tInfo = _info; }

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CSpringArm);
	CSpringArm();
	~CSpringArm();
};
