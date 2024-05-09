#include "pch.h"
#include "CCameraEffect.h"

#include <Engine\CRandomMgr.h>

CCameraEffect::CCameraEffect()
	: CScript((UINT)SCRIPT_TYPE::CAMERAEFFECT)
{
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
