#include "pch.h"
#include "CSpringArm.h"

#include <Engine\CRenderMgr.h>

CSpringArm::CSpringArm()
	: CScript((UINT)SCRIPT_TYPE::SPRINGARM), m_fDistance(300.f), m_vDir(1.f, 0.f, 0.f), m_vOffset(), m_bActive(true)
{
	AppendScriptParam("Active", SCRIPT_PARAM::BOOL, &m_bActive);
	AppendScriptParam("Smooth", SCRIPT_PARAM::BOOL, &m_bType);
	AppendScriptParam("Distance", SCRIPT_PARAM::FLOAT, &m_fDistance);
	AppendScriptParam("Speed", SCRIPT_PARAM::FLOAT, &m_fCamSpeed);
	AppendScriptParam("Offset", SCRIPT_PARAM::VEC3, &m_vOffset);
	AppendScriptParam("Cam", SCRIPT_PARAM::OBJECT, &m_pTargetCam);

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

	Vec3 vNewPos = GetOwner()->Transform()->GetWorldPos();
	vNewPos += m_vDir * m_fDistance;
	m_pTargetCam->Transform()->SetRelativePos(vNewPos);

	Vec3 vDir = GetOwner()->Transform()->GetWorldPos() - m_pTargetCam->Transform()->GetWorldPos();
	m_pTargetCam->Transform()->SetDir(vDir);
}

#include <Engine/CKeyMgr.h>

void CSpringArm::tick()
{
	if (!GetOwner() || !Transform() || /*!GetOwner()->GetParent() ||
		!GetOwner()->GetParent()->Transform() ||*/
		!m_pTargetCam)
		return;

	if (!m_bActive)
		return;

	Vec3 vNewPos = GetOwner()->Transform()->GetWorldPos();
	vNewPos += m_vDir * m_fDistance + m_vOffset;

	// false : Lerp, true : Smooth
	if (m_bType)
		vNewPos = Vec3::SmoothStep(m_pTargetCam->Transform()->GetWorldPos(), vNewPos, DT * m_fCamSpeed);
	else
		vNewPos = Vec3::Lerp(m_pTargetCam->Transform()->GetWorldPos(), vNewPos, DT * m_fCamSpeed);
	m_pTargetCam->Transform()->SetRelativePos(vNewPos);

	// 테스트용
	// Vec3 vPos = GetOwner()->Transform()->GetWorldPos();
	// if (KEY_PRESSED(A))
	//{
	//	vPos.z += m_fSpeed * DT;
	//}

	// if (KEY_PRESSED(D))
	//{
	//	vPos.z -= m_fSpeed * DT;
	// }
	// GetOwner()->Transform()->SetRelativePos(vPos);
}

void CSpringArm::SetTargetCamera(CCamera *_cam)
{
	m_pTargetCam = _cam;
}
