#pragma once
#include <Engine\CScript.h>

struct SpringArmInfo
{
	/// @brief 스프링 암이 향하는 각도
	Vec3 vDir = Vec3(0.f, 0.f, -1.f);
	/// @brief 스프링 암이 시작하는 지점 offset
	Vec3 vOffsetPos = Vec3(0.f, 0.f, 0.f);

	/// @brief 스프링 암의 최대 길이
	float fMaxDistance = 500.f;
	/// @brief 스프링 암의 길이
	float fDistance = fMaxDistance;

	/// @brief 카메라의 위치를 이동시키는 속도입니다.
	float fCamSpeed = 20.f;
	/// @brief 카메라의 물체를 바라보는 속도 입니다.
	float fCamRotSpeed = 20.f;
	/// @brief false : Lerp, true : Smooth
	bool Type = true;

public:
	friend ofstream& operator<<(ofstream& fout, const SpringArmInfo _info)
	{
		fout << _info.fMaxDistance << endl;
		fout << _info.fCamSpeed << endl;
		fout << _info.fCamRotSpeed << endl;
		fout << _info.Type << endl;
		return fout;
	}

	friend ifstream& operator>>(ifstream& fin, SpringArmInfo& _info)
	{
		fin >> _info.fMaxDistance;
		fin >> _info.fCamSpeed;
		fin >> _info.fCamRotSpeed;
		fin >> _info.Type;
		return fin;
	}
};

class CSpringArm : public CScript
{
private:
	/// @brief 스프링 암에 매달리는 오브젝트
	CGameObject* m_pTarget;
	/// @brief 활성화 여부
	bool m_bActive = true;

	SpringArmInfo m_tInfo;

	Vec3 m_vDirOffset;

public:
	void SetDirOffset(Vec3 _UD) { m_vDirOffset = _UD; }
	Vec3 GetDirOffset() { return m_vDirOffset; }

	/// @brief 스프링 암에 붙는 카메라를 세팅하는 함수입니다.
	void SetTargetObject(CGameObject* _pObject);

	CGameObject* GetTargetObject() { return m_pTarget; }
	/// @brief 스프링 암이 시작하는 지점부터 캠으로 향하는 방향을 세팅하는 함수입니다.
	void SetDir(Vec3 _dir) { m_tInfo.vDir = _dir; }
	/// @brief 스프링 암의 최대 길이를 지정하는 함수입니다. 현재 스프링 암의 길이도 같이 변경합니다.
	void SetDistance(float _dist)
	{
		m_tInfo.fMaxDistance = _dist;
		SetCurDistance(_dist);
	}

private:
	/// @brief 스프링 암의 길이를 지정하는 함수입니다. 최대 길이 캡을 보장합니다.
	void SetCurDistance(float _dist)
	{
		_dist < m_tInfo.fMaxDistance ? m_tInfo.fDistance = _dist : m_tInfo.fDistance = m_tInfo.fMaxDistance;
	}

public:
	/// @brief 캠이 따라붙는 속도를 설정하는 함수입니다.
	void SetCamSpeed(float _speed) { m_tInfo.fCamSpeed = _speed; }
	/// @brief 캠이 시작지점으로 회전하는 속도를 설정하는 함수입니다.
	void SetCamRotSpeed(float _speed) { m_tInfo.fCamRotSpeed = _speed; }
	/// @brief 스프링 암의 활성화 여부를 결정하는 함수입니다.
	void Activate(bool _active) { m_bActive = _active; }
	/// @brief 스프링 암의 활성화 여부를 반환하는 함수입니다.
	bool IsActivate() { return m_bActive; }
	/// @brief 카메라의 이동을 Smooth로 할지, Lerp로 할지 결정하는 함수입니다.
	/// @param _smooth true : Smooth, false : Lerp
	void SetMoveType(bool _type) { m_tInfo.Type = _type; }

	SpringArmInfo GetInfo() { return m_tInfo; }
	void		  SetInfo(const SpringArmInfo& _info, float _moveSpeed = 0.3f)
	{
		m_tInfo = _info;
		SetDistance(m_tInfo.fMaxDistance);

		m_tInfo.vDir.ToRadian();
		Transform()->Lerp(m_tInfo.vOffsetPos, true, m_tInfo.vDir, false, Vec3(), _moveSpeed);
	}

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CSpringArm);
	CSpringArm();
	~CSpringArm();

private:
	void Test();
};
