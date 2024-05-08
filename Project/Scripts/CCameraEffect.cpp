#include "pch.h"
#include "CCameraEffect.h"

#include <Engine\CRandomMgr.h>

CCameraEffect::CCameraEffect()
	: CScript((UINT)SCRIPT_TYPE::CAMERAEFFECT)
	, m_fZoomSpeed(10.f)
{
	//AppendScriptParam("ShakeDuration", SCRIPT_PARAM::FLOAT, &m_fShakeDuration, 0);
	//AppendScriptParam("ShakeTimer", SCRIPT_PARAM::FLOAT, &m_fShakeTimer, 0, 0, true);

	//AppendScriptParam("ReleaseDuration", SCRIPT_PARAM::FLOAT, &m_fReleaseDuration, 0);
	//AppendScriptParam("ReleaseTimer", SCRIPT_PARAM::FLOAT, &m_fReleaseTimer, 0, 0, true);

	//AppendScriptParam("ShakePosIntensity", SCRIPT_PARAM::VEC3, &m_vShakePosIntensity, 0);
	//AppendScriptParam("ShakeRotationIntensity", SCRIPT_PARAM::VEC3, &m_vShakeRotationIntensity, 0);

	//AppendScriptParam("ShakeFrequnecy", SCRIPT_PARAM::FLOAT, &m_fShakeFrequency, 0);
	//AppendScriptParam("ShakeFreqTimer", SCRIPT_PARAM::FLOAT, &m_fShakeFrequencyTimer, 0, 0, true);

	AppendScriptParam("ZoomSpeed", SCRIPT_PARAM::FLOAT, &m_fZoomSpeed, 0);

	RegistInitial();
}

CCameraEffect::~CCameraEffect()
{
}

void CCameraEffect::RegistInitial()
{
	if (!GetOwner() || !Transform() || !Camera()) return;
	m_vInitialPos = Transform()->GetRelativePos();
	m_vInitialRotation = Transform()->GetRelativeRotation();
	m_fInitialFOV = Camera()->GetFOV();
	m_fInitialScale = Camera()->GetScale();
}

void CCameraEffect::SendToInitial()
{
	if (!GetOwner() || !Transform()) return;
	Transform()->SetRelativePos(m_vInitialPos);
	Transform()->SetRelativeRotation(m_vInitialRotation);
	Camera()->SetFOV(m_fInitialFOV);
	Camera()->SetScale(m_fInitialScale);
}

void CCameraEffect::tick()
{
	// 테스트용 코드

	if (KEY_TAP(I)) {
		SendToInitial();
	}

	// 줌 인, 줌 아웃
	if (WHEEL_CHECK(WHEEL_UP)) {
		Camera()->SetFOV(Camera()->GetFOV() - m_fZoomSpeed * DT);
	}

	if (WHEEL_CHECK(WHEEL_DOWN)) {
		Camera()->SetFOV(Camera()->GetFOV() + m_fZoomSpeed * DT);
	}

	// 상태 세이브, 로드
	if (KEY_TAP(MBTN) && KEY_PRESSED(KEY::LCTRL)) {
		RegistInitial();
	}
	else if (KEY_TAP(MBTN)) {
		SendToInitial();
	}

}

void CCameraEffect::begin()
{
	RegistInitial();
}
