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
	AppendScriptParam("Offset", SCRIPT_PARAM::VEC3, &m_vOffset);
	AppendScriptParam("Offset", SCRIPT_PARAM::VEC3, &m_vDir);
	AppendScriptObject("Cam", &m_pTargetCam, COMPONENT_TYPE::CAMERA);

	// 테스트용
	// AppendScriptParam("characterspeed", SCRIPT_PARAM::FLOAT, &m_fSpeed);
}

CSpringArm::~CSpringArm()
{
}

void CSpringArm::begin()
{
	auto cam = CRenderMgr::GetInst()->GetMainCam();
	SetTargetCamera(cam);

	if (!GetOwner() || !Transform() || !m_pTargetCam)
		return;

	Vec3 vNewPos = GetOwner()->Transform()->GetWorldPos();
	vNewPos += m_vDir.Normalize() * m_fDistance;
	// m_pTargetCam->Transform()->SetRelativePos(vNewPos);
}

void CSpringArm::tick()
{
	if (!GetOwner() || !Transform() || !m_pTargetCam)
		return;

	if (!m_bActive)
		return;

	// 테스트 코드
	if (KEY_PRESSED(H))
	{
		m_vDir = m_vDir.RotateAround(Vec3(0.f, 1.f, 0.f), 1.f * DT);
	}
	if (KEY_PRESSED(G))
	{
		m_vDir = m_vDir.RotateAround(Vec3(0.f, 1.f, 0.f), -1.f * DT);
	}

	Vec3 vNewPos = GetOwner()->Transform()->GetWorldPos();
	vNewPos += m_vDir.Normalize() * m_fDistance + m_vOffset;
	Vec3 vNewDir = (GetOwner()->Transform()->GetWorldPos() + m_vOffset) - m_pTargetCam->Transform()->GetWorldPos();

	// false : Lerp, true : Smooth
	if (m_bType)
	{
		vNewPos = Vec3::SmoothStep(m_pTargetCam->Transform()->GetWorldPos(), vNewPos, DT * m_fCamSpeed);
		vNewDir = Vec3::SmoothStep(m_pTargetCam->Transform()->GetWorldDir(DIR_TYPE::FRONT), vNewDir, DT * m_fCamSpeed);
	}
	else
	{
		vNewPos = Vec3::Lerp(m_pTargetCam->Transform()->GetWorldPos(), vNewPos, DT * m_fCamSpeed);
		vNewDir = Vec3::Lerp(m_pTargetCam->Transform()->GetWorldDir(DIR_TYPE::FRONT), vNewDir, DT * m_fCamSpeed);
	}
	m_pTargetCam->Transform()->SetRelativePos(vNewPos);
	m_pTargetCam->Transform()->SetDir(vNewDir);
}

void CSpringArm::SetTargetCamera(CCamera* _cam)
{
	m_pTargetCam = _cam->GetOwner();
}
