#include "pch.h"
#include "CCameraShake.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CRandomMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CCamera.h"

CCameraShake::CCameraShake(CGameObject* _target)
	: m_fShakeDuration(1.f)
	, m_vShakePosIntensity(10.f, 10.f, 10.f)
	, m_vShakeRotationIntensity(0.f, 0.f, 10.f)
	, m_fShakeFrequency(6.f)
	, m_fReleaseDuration(0.2f)
	, m_pTargetCamera(_target)
{
}

CCameraShake::CCameraShake(CGameObject* _target, float _duration, Vec3 _posScale, Vec3 _rotScale, float _frequency,
						   float _releaseTime)
	: m_fShakeDuration(_duration)
	, m_vShakePosIntensity(_posScale)
	, m_vShakeRotationIntensity(_rotScale)
	, m_fShakeFrequency(_frequency)
	, m_fReleaseDuration(_releaseTime)
	, m_pTargetCamera(_target)
{
}

CCameraShake::~CCameraShake()
{
}

void CCameraShake::Releasing()
{
	if (m_fReleaseTimer <= 0.f)
	{
		m_bRelease = false;

		m_fReleaseTimer = 0.f;

		return;
	}

	Vec3 vNewPos = RoRMath::Lerp(m_vOriginPos, m_vStartPos, m_fReleaseTimer / m_fReleaseDuration);
	Vec3 vNewRot = RoRMath::Lerp(m_vOriginRot, m_vStartRotation, m_fReleaseTimer / m_fReleaseDuration);

	m_pTargetCamera->Transform()->SetRelativePos(vNewPos);
	m_pTargetCamera->Transform()->SetRelativeRotation(vNewRot);

	m_fReleaseTimer -= DT;
}

void CCameraShake::Shaking()
{
	if (m_fShakeTimer <= 0)
	{
		m_bShake   = false;
		m_bRelease = true;

		m_vStartPos		 = m_pTargetCamera->Transform()->GetRelativePos();
		m_vStartRotation = m_pTargetCamera->Transform()->GetRelativeRotation();

		m_fShakeTimer		   = 0.f;
		m_fShakeFrequencyTimer = 0.f;

		return;
	}

	if (m_fShakeFrequencyTimer <= 0)
	{
		m_fShakeFrequencyTimer += 1.f / m_fShakeFrequency;

		// Pos 선택
		m_vStartPos	 = m_pTargetCamera->Transform()->GetRelativePos();
		m_vTargetPos = m_vOriginPos;

		m_vTargetPos += CRandomMgr::GetInst()->GetRandomFloat() *
						m_pTargetCamera->Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_vShakePosIntensity.z;
		m_vTargetPos += CRandomMgr::GetInst()->GetRandomFloat() *
						m_pTargetCamera->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * m_vShakePosIntensity.x;
		m_vTargetPos += CRandomMgr::GetInst()->GetRandomFloat() *
						m_pTargetCamera->Transform()->GetWorldDir(DIR_TYPE::UP) * m_vShakePosIntensity.y;

		// 회전 선택
		m_vStartRotation  = m_pTargetCamera->Transform()->GetRelativeRotation();
		m_vTargetRotation = m_vOriginRot;

		m_vTargetRotation.x +=
			CRandomMgr::GetInst()->GetRandomFloat() * XMConvertToRadians(m_vShakeRotationIntensity.x);
		m_vTargetRotation.y +=
			CRandomMgr::GetInst()->GetRandomFloat() * XMConvertToRadians(m_vShakeRotationIntensity.y);
		m_vTargetRotation.z +=
			CRandomMgr::GetInst()->GetRandomFloat() * XMConvertToRadians(m_vShakeRotationIntensity.z);
	}
	Vec3 vNewPos = RoRMath::Lerp(m_vTargetPos, m_vStartPos, m_fShakeFrequencyTimer * m_fShakeFrequency);
	Vec3 vNewRot = RoRMath::Lerp(m_vTargetRotation, m_vStartRotation, m_fShakeFrequencyTimer * m_fShakeFrequency);

	m_pTargetCamera->Transform()->SetRelativePos(vNewPos);
	m_pTargetCamera->Transform()->SetRelativeRotation(vNewRot);

	m_fShakeTimer -= DT;
	m_fShakeFrequencyTimer -= DT;
}

void CCameraShake::Shake()
{
	m_bShake   = true;
	m_bRelease = false;

	m_fReleaseTimer		   = m_fReleaseDuration;
	m_fShakeTimer		   = m_fShakeDuration;
	m_fShakeFrequencyTimer = 0;

	m_vOriginPos = m_pTargetCamera->Transform()->GetRelativePos();
	m_vOriginRot = m_pTargetCamera->Transform()->GetRelativeRotation();
}

void CCameraShake::SetShakeAttribute(CGameObject* _target, float _duration, Vec3 _posScale, Vec3 _rotScale,
									 float _frequency, float _releaseTime)
{
	m_pTargetCamera			  = _target;
	m_fShakeDuration		  = _duration;
	m_vShakePosIntensity	  = _posScale;
	m_vShakeRotationIntensity = _rotScale;
	m_fShakeFrequency		  = _frequency;
	m_fReleaseDuration		  = _releaseTime;
}

void CCameraShake::finaltick()
{
	if (!m_pTargetCamera || !m_pTargetCamera->Transform() || !m_pTargetCamera->Camera())
		return;

	// 테스트 코드
	if (KEY_TAP(T))
	{
		Shake();
	}

	// 카메라 쉐이킹
	if (m_bShake)
	{
		Shaking();
	}
	else if (m_bRelease)
	{
		Releasing();
	}
}

#define TagShakeDuration "[ShakeDuration]"
#define TagShakePosIntens "[PosIntensity]"
#define TagShakeRotIntens "[RotIntensity]"
#define TagShakeFrequency "[Frequency]"
#define TagShakeRelease "[ReleaseDuration]"

ofstream& operator<<(ofstream& fout, const CCameraShake& _shake)
{
	fout << TagShakeDuration << endl;
	fout << _shake.m_fShakeDuration << endl;

	fout << TagShakePosIntens << endl;
	fout << _shake.m_vShakePosIntensity << endl;

	fout << TagShakeRotIntens << endl;
	fout << _shake.m_vShakeRotationIntensity << endl;

	fout << TagShakeFrequency << endl;
	fout << _shake.m_fShakeFrequency << endl;

	fout << TagShakeRelease << endl;
	fout << _shake.m_fReleaseDuration << endl;

	return fout;
}

ifstream& operator>>(ifstream& fin, CCameraShake& _shake)
{
	Utils::GetLineUntilString(fin, TagShakeDuration);
	fin >> _shake.m_fShakeDuration;

	Utils::GetLineUntilString(fin, TagShakePosIntens);
	fin >> _shake.m_vShakePosIntensity;

	Utils::GetLineUntilString(fin, TagShakeRotIntens);
	fin >> _shake.m_vShakeRotationIntensity;

	Utils::GetLineUntilString(fin, TagShakeFrequency);
	fin >> _shake.m_fShakeFrequency;

	Utils::GetLineUntilString(fin, TagShakeRelease);
	fin >> _shake.m_fShakeDuration;

	return fin;
}
