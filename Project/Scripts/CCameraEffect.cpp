#include "pch.h"
#include "CCameraEffect.h"

#include <Engine\CRandomMgr.h>

CCameraEffect::CCameraEffect()
	: CScript((UINT)SCRIPT_TYPE::CAMERAEFFECT)
	, m_fShakeDuration(1.f)
	, m_vShakePosIntensity(10.f, 10.f, 10.f)
	, m_vShakeRotationIntensity(0.f, 0.f, 10.f)
	, m_fShakeFrequency(6.f)
	, m_fReleaseDuration(0.2f)
	, m_fZoomSpeed(10.f)
	, m_fDoubleClickDuration(0.32f)
{
	AppendScriptParam("ShakeDuration", SCRIPT_PARAM::FLOAT, &m_fShakeDuration, 0);
	AppendScriptParam("ShakeTimer", SCRIPT_PARAM::FLOAT, &m_fShakeTimer, 0, 0, true);

	AppendScriptParam("ReleaseDuration", SCRIPT_PARAM::FLOAT, &m_fReleaseDuration, 0);
	AppendScriptParam("ReleaseTimer", SCRIPT_PARAM::FLOAT, &m_fReleaseTimer, 0, 0, true);

	AppendScriptParam("ShakePosIntensity", SCRIPT_PARAM::VEC3, &m_vShakePosIntensity, 0);
	AppendScriptParam("ShakeRotationIntensity", SCRIPT_PARAM::VEC3, &m_vShakeRotationIntensity, 0);

	AppendScriptParam("ShakeFrequnecy", SCRIPT_PARAM::FLOAT, &m_fShakeFrequency, 0);
	AppendScriptParam("ShakeFreqTimer", SCRIPT_PARAM::FLOAT, &m_fShakeFrequencyTimer, 0, 0, true);

	RegistInitial();
}

CCameraEffect::~CCameraEffect()
{
}

void CCameraEffect::Shaking()
{
	if (m_fShakeTimer <= 0)
	{
		m_bShake = false;
		m_bRelease = true;

		m_vStartPos = Transform()->GetRelativePos();
		m_vStartRotation = Transform()->GetRelativeRotation();

		m_fShakeTimer = 0.f;
		m_fShakeFrequencyTimer = 0.f;
		
		return;
	}

	if (m_fShakeFrequencyTimer <= 0) {
		m_fShakeFrequencyTimer += 1.f / m_fShakeFrequency;

		// Pos 선택
		m_vStartPos = Transform()->GetRelativePos();
		m_vTargetPos = m_vOriginPos;

		m_vTargetPos += CRandomMgr::GetInst()->GetRandomFloat() * Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_vShakePosIntensity.z;
		m_vTargetPos += CRandomMgr::GetInst()->GetRandomFloat() * Transform()->GetWorldDir(DIR_TYPE::RIGHT) * m_vShakePosIntensity.x;
		m_vTargetPos += CRandomMgr::GetInst()->GetRandomFloat() * Transform()->GetWorldDir(DIR_TYPE::UP) * m_vShakePosIntensity.y;

		// 회전 선택
		m_vStartRotation = Transform()->GetRelativeRotation();
		m_vTargetRotation = m_vOriginRot;

		m_vTargetRotation.x += CRandomMgr::GetInst()->GetRandomFloat() * XMConvertToRadians(m_vShakeRotationIntensity.x);
		m_vTargetRotation.y += CRandomMgr::GetInst()->GetRandomFloat() * XMConvertToRadians(m_vShakeRotationIntensity.y);
		m_vTargetRotation.z += CRandomMgr::GetInst()->GetRandomFloat() * XMConvertToRadians(m_vShakeRotationIntensity.z);
	}
	Vec3 vNewPos = RoRMath::Lerp(m_vTargetPos, m_vStartPos, m_fShakeFrequencyTimer * m_fShakeFrequency);
	Vec3 vNewRot = RoRMath::Lerp(m_vTargetRotation, m_vStartRotation, m_fShakeFrequencyTimer * m_fShakeFrequency);

	Transform()->SetRelativePos(vNewPos);
	Transform()->SetRelativeRotation(vNewRot);

	m_fShakeTimer -= DT;
	m_fShakeFrequencyTimer -= DT;
}

void CCameraEffect::Releasing()
{
	if (m_fReleaseTimer <= 0.f) {
		m_bRelease = false;

		m_fReleaseTimer = 0.f;

		return;
	}

	Vec3 vNewPos = RoRMath::Lerp(m_vOriginPos, m_vStartPos, m_fReleaseTimer / m_fReleaseDuration);
	Vec3 vNewRot = RoRMath::Lerp(m_vOriginRot, m_vStartRotation, m_fReleaseTimer / m_fReleaseDuration);

	Transform()->SetRelativePos(vNewPos);
	Transform()->SetRelativeRotation(vNewRot);

	m_fReleaseTimer -= DT;
}

void CCameraEffect::Shake(float _duration, Vec3 _scale, float _releaseTime)
{
	m_bShake = true;
	m_bRelease = false;

	m_fReleaseTimer = _releaseTime;

	m_fShakeTimer = _duration;
	m_vShakePosIntensity = _scale;
	m_fShakeFrequencyTimer = 0;

	m_vOriginPos = Transform()->GetRelativePos();
	m_vOriginRot = Transform()->GetRelativeRotation();
}

void CCameraEffect::RegistInitial()
{
	if (!GetOwner() || !Transform()) return;
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
	if (!GetOwner() || !Transform()) return;
	Transform()->SetRelativePos(m_vInitialPos);
	Transform()->SetRelativeRotation(m_vInitialRotation);
}

void CCameraEffect::tick()
{
	// 테스트용 코드
	if (KEY_TAP(T)) 
	{
		Shake(m_fShakeDuration, m_vShakePosIntensity, m_fReleaseDuration);
	}
	if (KEY_TAP(I)) {
		SendToInitial();
	}

	if (WHEEL_CHECK(WHEEL_UP)) {
		Camera()->SetFOV(Camera()->GetFOV() - m_fZoomSpeed * DT);
	}

	if (WHEEL_CHECK(WHEEL_DOWN)) {
		Camera()->SetFOV(Camera()->GetFOV() + m_fZoomSpeed * DT);
	}

	if (m_bDoubleClickCheck) {
		m_fDoubleClickDurationTimer -= DT;

		if (KEY_TAP(MBTN)) {
			RegistInitial();
			m_bDoubleClickCheck = false;
			m_fDoubleClickDurationTimer = 0.f;
		}
		else if (m_fDoubleClickDurationTimer <= 0.f) {
			m_fDoubleClickDurationTimer = 0.f;
			m_bDoubleClickCheck = false;
			SendToInitial();
		}
	}

	if (KEY_TAP(MBTN)) {
		m_bDoubleClickCheck = true;
		m_fDoubleClickDurationTimer = m_fDoubleClickDuration;
	}

	// 카메라 쉐이킹
	if (m_bShake) 
	{
		Shaking();
	}
	else if (m_bRelease) {
		Releasing();
	}
}

void CCameraEffect::begin()
{
	RegistInitial();
}
