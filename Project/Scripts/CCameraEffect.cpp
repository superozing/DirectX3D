#include "pch.h"
#include "CCameraEffect.h"

#include <Engine\CRandomMgr.h>

CCameraEffect::CCameraEffect()
	: CScript((UINT)SCRIPT_TYPE::CAMERAEFFECT)
	, m_fShakeDuration(1.f)
	, m_vShakePosIntensity(10.f, 10.f)
	, m_vShakeRotationIntensity(0.f, 0.f, 10.f)
	, m_fShakeFrequency(6.f)
{
	AppendScriptParam("ShakeDuration", SCRIPT_PARAM::FLOAT, &m_fShakeDuration, 0);
	AppendScriptParam("ShakeTimer", SCRIPT_PARAM::FLOAT, &m_fShakeTimer, 0, 0, true);
	AppendScriptParam("ShakePosIntensity", SCRIPT_PARAM::VEC2, &m_vShakePosIntensity, 0);
	AppendScriptParam("ShakeRotationIntensity", SCRIPT_PARAM::VEC3, &m_vShakeRotationIntensity, 0);

	AppendScriptParam("ShakeFrequnecy", SCRIPT_PARAM::FLOAT, &m_fShakeFrequency, 0);
	AppendScriptParam("ShakeFreqTimer", SCRIPT_PARAM::FLOAT, &m_fShakeFrequencyTimer, 0, 0, true);
}

CCameraEffect::~CCameraEffect()
{
}

void CCameraEffect::Shaking()
{
	if (m_fShakeTimer <= 0)
	{
		m_bShake = false;
		m_fShakeTimer = 0.f;
		m_fShakeFrequencyTimer = 0.f;

		return;
	}

	if (m_fShakeFrequencyTimer <= 0) {
		m_fShakeFrequencyTimer += 1.f / m_fShakeFrequency;

		// Pos 선택
		m_vStartPos = Transform()->GetRelativePos();
		m_vTargetPos = m_vStartPos;

		m_vTargetPos.x += CRandomMgr::GetInst()->GetRandomFloat() * m_vShakePosIntensity.x;
		m_vTargetPos.y += CRandomMgr::GetInst()->GetRandomFloat() * m_vShakePosIntensity.y;

		// 회전 선택
		m_vStartRotation = Transform()->GetRelativeRotation();
		m_vTargetRotation = m_vStartRotation;

		m_vTargetRotation.x = CRandomMgr::GetInst()->GetRandomFloat() * XMConvertToRadians(m_vShakeRotationIntensity.x);
		m_vTargetRotation.y = CRandomMgr::GetInst()->GetRandomFloat() * XMConvertToRadians(m_vShakeRotationIntensity.y);
		m_vTargetRotation.z = CRandomMgr::GetInst()->GetRandomFloat() * XMConvertToRadians(m_vShakeRotationIntensity.z);
	}
	Vec3 vNewPos = RoRMath::Lerp(m_vTargetPos, m_vStartPos, m_fShakeFrequencyTimer * m_fShakeFrequency);
	Vec3 vNewRot = RoRMath::Lerp(m_vTargetRotation, m_vStartRotation, m_fShakeFrequencyTimer * m_fShakeFrequency);

	Transform()->SetRelativePos(vNewPos);
	Transform()->SetRelativeRotation(vNewRot);

	m_fShakeTimer -= DT;
	m_fShakeFrequencyTimer -= DT;
}

void CCameraEffect::Shake(float _duration, Vec2 _scale)
{
	m_bShake = true;

	m_fShakeTimer = _duration;
	m_vShakePosIntensity = _scale;
	m_fShakeFrequencyTimer = 0;
}

void CCameraEffect::RegistInitial()
{
	m_vInitialPos = Transform()->GetRelativePos();
	m_vInitialRotation = Transform()->GetRelativeRotation();
}

void CCameraEffect::RegistInitial(Vec3 _pos, Vec3 _rot)
{
	m_vInitialPos = _pos;
	m_vInitialRotation = _rot;
}

void CCameraEffect::SendToInitial()
{
	Transform()->SetRelativePos(m_vInitialPos);
	Transform()->SetRelativeRotation(m_vInitialRotation);
}

void CCameraEffect::tick()
{
	// 테스트용 코드
	if (KEY_TAP(T)) 
	{
		Shake(m_fShakeDuration, m_vShakePosIntensity);
	}
	if (KEY_TAP(I)) {
		SendToInitial();
	}


	// 카메라 쉐이킹
	if (m_bShake) 
	{
		Shaking();
	}
}

void CCameraEffect::begin()
{
	RegistInitial();
}
