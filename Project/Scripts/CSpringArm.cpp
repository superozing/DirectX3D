#include "pch.h"
#include "CSpringArm.h"

#include <Engine\CRenderMgr.h>

CSpringArm::CSpringArm()
	: CScript((UINT)SCRIPT_TYPE::SPRINGARM)
	, m_fDistance(300.f)
	, m_vDir(1.f, 0.f, 0.f)
	, m_vOffset()
	, m_bActive(true)
	, m_bType(true)
	, m_fCamSpeed(20.f)
{
	AppendScriptParam("Active", SCRIPT_PARAM::BOOL, &m_bActive);
	AppendScriptParam("Smooth", SCRIPT_PARAM::BOOL, &m_bType);
	AppendScriptParam("Distance", SCRIPT_PARAM::FLOAT, &m_fDistance);
	AppendScriptParam("Speed", SCRIPT_PARAM::FLOAT, &m_fCamSpeed);
	AppendScriptParam("RotSpeed", SCRIPT_PARAM::FLOAT, &m_fCamRotateSpeed);
	AppendScriptParam("TargetPos", SCRIPT_PARAM::VEC3, &m_vTargetPos);
	AppendScriptParam("Offset", SCRIPT_PARAM::VEC3, &m_vOffset);
	AppendScriptParam("Direction", SCRIPT_PARAM::VEC3, &m_vDir);
	AppendScriptObject("Cam", &m_pTargetCam, COMPONENT_TYPE::CAMERA);
}

CSpringArm::~CSpringArm()
{
}

void CSpringArm::begin()
{
	auto cam = CRenderMgr::GetInst()->GetMainCam();
	SetTargetCamera(cam);
	m_vTargetPos = Transform()->GetWorldPos() + m_vOffset + m_vDir.Normalize() * m_fDistance;
}

void CSpringArm::tick()
{
	if (!GetOwner() || !Transform() || !m_pTargetCam || !m_bActive)
		return;

	static float fCurDistance = m_fDistance;
	static Vec3	 vCurDir	  = m_vDir;
	static Vec3	 vCurPos	  = m_vTargetPos;

	// 테스트 코드
	// 머지 후 playerscript의 바깥에서 해야하는 역할
	if (KEY_PRESSED(H))
	{
		m_vDir = m_vDir.RotateAround(Vec3(0.f, 1.f, 0.f), 1.f * DT);
	}
	if (KEY_PRESSED(G))
	{
		m_vDir = m_vDir.RotateAround(Vec3(0.f, 1.f, 0.f), -1.f * DT);
	}

	if (KEY_TAP(J))
	{
		m_fDistance = 100.f;
	}
	if (KEY_TAP(K))
	{
		m_fDistance = 300.f;
	}
	// 테스트 코드 끝

	Vec3 vNewPos = GetOwner()->Transform()->GetWorldPos();
	vNewPos += m_vDir.Normalize() * fCurDistance + m_vOffset;
	Vec3 vNewDir = (GetOwner()->Transform()->GetWorldPos() + m_vOffset) - m_pTargetCam->Transform()->GetWorldPos();

	// false : Lerp, true : Smooth
	if (m_bType)
	{
		vNewPos = Vec3::SmoothStep(m_pTargetCam->Transform()->GetWorldPos(), vNewPos, DT * m_fCamSpeed);
		vNewDir =
			Vec3::SmoothStep(m_pTargetCam->Transform()->GetWorldDir(DIR_TYPE::FRONT), vNewDir, DT * m_fCamRotateSpeed);
		fCurDistance = RoRMath::SmoothStep(fCurDistance, m_fDistance, DT * m_fCamSpeed);
	}
	else
	{
		vNewPos = Vec3::Lerp(m_pTargetCam->Transform()->GetWorldPos(), vNewPos, DT * m_fCamSpeed);
		vNewDir = Vec3::Lerp(m_pTargetCam->Transform()->GetWorldDir(DIR_TYPE::FRONT), vNewDir, DT * m_fCamRotateSpeed);
		fCurDistance = RoRMath::Lerp(fCurDistance, m_fDistance, DT * m_fCamSpeed);
	}

	m_pTargetCam->Transform()->SetRelativePos(vNewPos);
	m_pTargetCam->Transform()->SetDir(vNewDir);
}

void CSpringArm::SetTargetCamera(CCamera* _cam)
{
	m_pTargetCam = _cam->GetOwner();
}
