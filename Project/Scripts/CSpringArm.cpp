#include "pch.h"
#include "CSpringArm.h"

#include <Engine\CRenderMgr.h>
#include <Engine\CRandomMgr.h>
#include <Engine/CPhysXMgr.h>

CSpringArm::CSpringArm()
	: CScript((UINT)SCRIPT_TYPE::SPRINGARM)
	, m_tInfo{}

{
	AppendScriptParam("Active", SCRIPT_PARAM::BOOL, &m_bActive);
	AppendScriptParam("Smooth", SCRIPT_PARAM::BOOL, &m_tInfo.Type);
	AppendScriptParam("Distance", SCRIPT_PARAM::FLOAT, &m_tInfo.fMaxDistance);
	AppendScriptParam("Speed", SCRIPT_PARAM::FLOAT, &m_tInfo.fCamSpeed);
	AppendScriptParam("RotSpeed", SCRIPT_PARAM::FLOAT, &m_tInfo.fCamRotSpeed);

	AppendScriptObject("Cam", &m_pTarget, COMPONENT_TYPE::TRANSFORM);
}

CSpringArm::~CSpringArm()
{
}

void CSpringArm::begin()
{
	if (!GetOwner() || !Transform() || !m_pTarget || !m_bActive)
		return;
	SetDistance(m_tInfo.fMaxDistance);

	m_tInfo.vDir;
	Transform()->Lerp(m_tInfo.vOffsetPos, true, m_tInfo.vDir, false, Vec3(), 0.f);

	Matrix worldMat	  = Transform()->GetWorldMat();
	Vec3   vCenterPos = Transform()->GetWorldPos();
	Vec3   vTargetPos = m_pTarget->Transform()->GetWorldPos();
	Vec3   vDir		  = XMVector3TransformNormal(Vec3(0.f, 0.f, 1.f), worldMat);

	Vec3 vNewPos = vCenterPos + vDir * m_tInfo.fDistance;
	Vec3 vNewDir = vCenterPos - vTargetPos;

	m_pTarget->Transform()->SetRelativePos(vNewPos);
	m_pTarget->Transform()->SetDir(vNewDir);
}

void CSpringArm::tick()
{
	if (!GetOwner() || !Transform() || !m_pTarget || !m_bActive)
		return;

	static float fCurDistance = m_tInfo.fDistance;

	Matrix worldMat = Transform()->GetWorldMat();

	Vec3 vDir = XMVector3TransformNormal(Vec3(0.f, 0.f, 1.f), worldMat);

	Vec3 vCenterPos = Transform()->GetWorldPos();
	Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();

	Vec3 vNewPos = vCenterPos + vDir * m_tInfo.fDistance;
	Vec3 vNewDir = vCenterPos - vTargetPos;
	vNewDir.Normalize();

	// false : Lerp, true : Smooth
	if (m_tInfo.Type)
	{
		vNewPos		 = Vec3::SmoothStep(m_pTarget->Transform()->GetWorldPos(), vNewPos, DT * m_tInfo.fCamSpeed);
		fCurDistance = RoRMath::SmoothStep(fCurDistance, m_tInfo.fDistance, DT * m_tInfo.fCamSpeed);
	}
	else
	{
		vNewPos		 = Vec3::Lerp(m_pTarget->Transform()->GetWorldPos(), vNewPos, DT * m_tInfo.fCamSpeed);
		fCurDistance = RoRMath::Lerp(fCurDistance, m_tInfo.fDistance, DT * m_tInfo.fCamSpeed);
	}

	tRoRHitInfo tHitInfo;

	int rayFlag = (int)RayCastDebugFlag::RayLineVisible | RayCastDebugFlag::EndPointVisible;
	if (CPhysXMgr::GetInst()->PerformRaycast(vNewPos, -vNewDir, tHitInfo, (UINT)LAYER::LAYER_PLAYER_CAMERA_RAY,
											 rayFlag))
	{
		float length = (tHitInfo.vHitPos - vNewPos).Length();
		SetCurDistance(length);
	}
	else
	{
		SetCurDistance(m_tInfo.fMaxDistance);
	}

	m_pTarget->Transform()->SetRelativePos(vNewPos);
	m_pTarget->Transform()->SetDir(vNewDir);
	Vec3 vRot		= m_pTarget->Transform()->GetRelativeRotation();
	Vec3 vDirOffset = Vec3(m_vDirOffset.y, m_vDirOffset.x, m_vDirOffset.z);
	vDirOffset.ToRadian();
	m_pTarget->Transform()->SetRelativeRotation(vRot + vDirOffset);

	// 카메라 쉐이킹
	if (m_bShake)
	{
		SpringArmShaking();
	}
	else if (m_bRelease)
	{
		SpringArmReleasing();
	}
}

void CSpringArm::SpringArmReleasing()
{
	if (m_fReleaseTimer <= 0.f)
	{
		m_bRelease = false;

		m_fReleaseTimer = 0.f;

		Transform()->SetRelativePos(m_vOriginPos);
		Transform()->SetRelativeRotation(m_vOriginRot);

		return;
	}

	Vec3 vNewPos = RoRMath::Lerp(m_vOriginPos, m_vStartPos, m_fReleaseTimer / m_fReleaseDuration);
	Vec3 vNewRot = RoRMath::Lerp(m_vOriginRot, m_vStartRotation, m_fReleaseTimer / m_fReleaseDuration);

	Transform()->SetRelativePos(vNewPos);
	Transform()->SetRelativeRotation(vNewRot);

	m_fReleaseTimer -= DT;
}

void CSpringArm::SpringArmShake()
{
	if (!m_bShake && !m_bRelease)
	{
		m_vOriginPos = Transform()->GetRelativePos();
		m_vOriginRot = Transform()->GetRelativeRotation();
	}

	m_bShake   = true;
	m_bRelease = false;

	m_fReleaseTimer		   = m_fReleaseDuration;
	m_fShakeTimer		   = m_fShakeDuration;
	m_fShakeFrequencyTimer = 0;
}

void CSpringArm::SpringArmShaking()
{
	if (m_fShakeTimer <= 0)
	{
		m_bShake   = false;
		m_bRelease = true;

		m_vStartPos		 = Transform()->GetRelativePos();
		m_vStartRotation = Transform()->GetRelativeRotation();

		m_fShakeTimer		   = 0.f;
		m_fShakeFrequencyTimer = 0.f;

		return;
	}

	if (m_fShakeFrequencyTimer <= 0)
	{
		m_fShakeFrequencyTimer += 1.f / m_fShakeFrequency;

		// Pos 선택
		m_vStartPos	 = Transform()->GetRelativePos();
		m_vTargetPos = m_vOriginPos;

		m_vTargetPos += CRandomMgr::GetInst()->GetRandomFloat() * Transform()->GetWorldDir(DIR_TYPE::FRONT) *
						m_vShakePosIntensity.z;
		m_vTargetPos += CRandomMgr::GetInst()->GetRandomFloat() * Transform()->GetWorldDir(DIR_TYPE::RIGHT) *
						m_vShakePosIntensity.x;
		m_vTargetPos +=
			CRandomMgr::GetInst()->GetRandomFloat() * Transform()->GetWorldDir(DIR_TYPE::UP) * m_vShakePosIntensity.y;

		// 회전 선택
		m_vStartRotation  = Transform()->GetRelativeRotation();
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

	Transform()->SetRelativePos(vNewPos);
	Transform()->SetRelativeRotation(vNewRot);

	m_fShakeTimer -= DT;
	m_fShakeFrequencyTimer -= DT;
}

void CSpringArm::SetShakeAttribute(float _duration, Vec3 _posScale, Vec3 _rotScale, float _frequency,
								   float _releaseTime)
{
	m_fShakeDuration		  = _duration;
	m_vShakePosIntensity	  = _posScale;
	m_vShakeRotationIntensity = _rotScale;
	m_fShakeFrequency		  = _frequency;
	m_fReleaseDuration		  = _releaseTime;
}

#define TagSpringArm "[Info]"
#define TagActive "[ActiveFitures]"

void CSpringArm::SaveToFile(ofstream& fout)
{
	fout << TagSpringArm << endl;
	fout << m_tInfo << endl;

	fout << TagActive << endl;
	fout << m_bActive << endl;
}

void CSpringArm::LoadFromFile(ifstream& fin)
{
	// Utils::GetLineUntilString(fin, TagSpringArm);
	// fin >> m_tInfo;

	// Utils::GetLineUntilString(fin, TagActive);
	// fin >> m_bActive;
}

void CSpringArm::SetTargetObject(CGameObject* _pObject)
{
	m_pTarget = _pObject;
}

void CSpringArm::Test()
{
	// if (KEY_PRESSED(H))
	//{
	//	m_tInfo.vDir = m_tInfo.vDir.RotateAround(Vec3(0.f, 1.f, 0.f), 1.f * DT);
	//}
	// if (KEY_PRESSED(G))
	//{
	//	m_tInfo.vDir = m_tInfo.vDir.RotateAround(Vec3(0.f, 1.f, 0.f), -1.f * DT);
	//}
}
