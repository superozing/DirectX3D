#include "pch.h"
#include "CCameraEffect.h"

#include <Engine\CRandomMgr.h>

CCameraEffect::CCameraEffect()
	: CScript((UINT)SCRIPT_TYPE::CAMERAEFFECT)
	, m_fShakeDuration(1.f)
	, m_vShakeIntensity(10.f, 10.f)
	, m_fShakeFrequency(6.f)
{
	AppendScriptParam("ShakeDuration", SCRIPT_PARAM::FLOAT, &m_fShakeDuration, 0);
	AppendScriptParam("ShakeTimer", SCRIPT_PARAM::FLOAT, &m_fShakeTimer, 0, 0, true);
	AppendScriptParam("ShakeScale", SCRIPT_PARAM::VEC2, &m_vShakeIntensity, 0);

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
		m_vStartPos = Transform()->GetRelativePos();

		m_vTargetPos = m_vStartPos;
		m_vTargetPos.x += CRandomMgr::GetInst()->GetRandomFloat() * m_vShakeIntensity.x;
		m_vTargetPos.y += CRandomMgr::GetInst()->GetRandomFloat() * m_vShakeIntensity.y;
	}
	Vec3 vNewPos = RoRMath::Lerp(m_vTargetPos, m_vStartPos, m_fShakeFrequencyTimer * m_fShakeFrequency);

	Transform()->SetRelativePos(vNewPos);

	m_fShakeTimer -= DT;
	m_fShakeFrequencyTimer -= DT;
}

void CCameraEffect::Shake(float _duration, Vec2 _scale)
{
	m_bShake = true;

	m_fShakeTimer = _duration;
	m_vShakeIntensity = _scale;
	m_fShakeFrequencyTimer = 0;
}


void CCameraEffect::tick()
{
	// 테스트용 코드
	if (KEY_TAP(T)) 
	{
		Shake(m_fShakeDuration, m_vShakeIntensity);
	}

	// 카메라 쉐이킹
	if (m_bShake) 
	{
		Shaking();
	}
}