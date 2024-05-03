#include "pch.h"
#include "CCameraEffect.h"

#include <Engine\CRandomMgr.h>

CCameraEffect::CCameraEffect()
	: CScript((UINT)SCRIPT_TYPE::CAMERAEFFECT)
	, m_fShakeDuration(1.f)
	, m_fShakeScale(1.f)
{
	AppendScriptParam("ShakeDuration", SCRIPT_PARAM::FLOAT, &m_fShakeDuration, 0);
	AppendScriptParam("ShakeTimer", SCRIPT_PARAM::FLOAT, &m_fShakeTimer, 0, 0, true);
	AppendScriptParam("ShakeScale", SCRIPT_PARAM::FLOAT, &m_fShakeScale, 0);
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
		return;
	}

	Vec3 vPos = Transform()->GetRelativePos();
	vPos.x = CRandomMgr::GetInst()->GetRandomFloat() * m_fShakeScale;
	vPos.y = CRandomMgr::GetInst()->GetRandomFloat() * m_fShakeScale;

	Transform()->SetRelativePos(vPos);

	m_fShakeTimer -= DT;
}

void CCameraEffect::Shake(float _duration, float _scale)
{
	m_bShake = true;

	m_fShakeTimer = _duration;
	m_fShakeScale = _scale;
}


void CCameraEffect::tick()
{
	// 테스트용 코드
	if (KEY_TAP(T)) 
	{
		Shake(m_fShakeDuration, m_fShakeScale);
	}

	// 카메라 쉐이킹
	if (m_bShake) 
	{
		Shaking();
	}
}