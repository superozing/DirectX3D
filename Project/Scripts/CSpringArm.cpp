#include "pch.h"
#include "CSpringArm.h"

#include <Engine\CRenderMgr.h>
#include <Engine/CPhysXMgr.h>

CSpringArm::CSpringArm()
	: CScript((UINT)SCRIPT_TYPE::SPRINGARM)
	, m_tInfo{}

{
	AppendScriptParam("Active", SCRIPT_PARAM::BOOL, &m_bActive);
	AppendScriptParam("Smooth", SCRIPT_PARAM::BOOL, &m_tInfo.Type);
	AppendScriptParam("Distance", SCRIPT_PARAM::FLOAT, &m_tInfo.fDistance);
	AppendScriptParam("Speed", SCRIPT_PARAM::FLOAT, &m_tInfo.fCamSpeed);
	AppendScriptParam("RotSpeed", SCRIPT_PARAM::FLOAT, &m_tInfo.fCamRotSpeed);
	AppendScriptParam("PosOffset", SCRIPT_PARAM::VEC3, &m_tInfo.vOffset);
	AppendScriptParam("DirOffset", SCRIPT_PARAM::VEC3, &m_tInfo.vDirOffset);
	AppendScriptParam("Direction", SCRIPT_PARAM::VEC3, &m_tInfo.vDir);
	AppendScriptObject("Cam", &m_pTargetCam, COMPONENT_TYPE::TRANSFORM);
}

CSpringArm::~CSpringArm()
{
}

void CSpringArm::begin()
{
	auto cam = CRenderMgr::GetInst()->GetMainCam();
	// SetTargetCamera(cam);
}

void CSpringArm::tick()
{
	if (!GetOwner() || !Transform() || !m_pTargetCam || !m_bActive)
		return;

	static float fCurDistance = m_tInfo.fDistance;

	// 테스트 코드
	// 머지 후 playerscript의 바깥에서 해야하는 역할
	if (KEY_PRESSED(H))
	{
		m_tInfo.vDir = m_tInfo.vDir.RotateAround(Vec3(0.f, 1.f, 0.f), 1.f * DT);
	}
	if (KEY_PRESSED(G))
	{
		m_tInfo.vDir = m_tInfo.vDir.RotateAround(Vec3(0.f, 1.f, 0.f), -1.f * DT);
	}

	if (KEY_TAP(J))
	{
		SetDistance(100.f);
	}
	if (KEY_TAP(K))
	{
		SetDistance(300.f);
	}
	// 테스트 코드 끝

	Vec3 vNewPos =
		(GetOwner()->Transform()->GetWorldPos() + m_tInfo.vOffset) + m_tInfo.vDir.Normalize() * m_tInfo.fDistance;
	Vec3 vNewDir = (GetOwner()->Transform()->GetWorldPos() + m_tInfo.vOffset + m_tInfo.vDirOffset) -
				   m_pTargetCam->Transform()->GetWorldPos();

	// false : Lerp, true : Smooth
	if (m_tInfo.Type)
	{
		vNewPos		 = Vec3::SmoothStep(m_pTargetCam->Transform()->GetWorldPos(), vNewPos, DT * m_tInfo.fCamSpeed);
		vNewDir		 = Vec3::SmoothStep(m_pTargetCam->Transform()->GetWorldDir(DIR_TYPE::FRONT), vNewDir,
										DT * m_tInfo.fCamRotSpeed);
		fCurDistance = RoRMath::SmoothStep(fCurDistance, m_tInfo.fDistance, DT * m_tInfo.fCamSpeed);
	}
	else
	{
		vNewPos = Vec3::Lerp(m_pTargetCam->Transform()->GetWorldPos(), vNewPos, DT * m_tInfo.fCamSpeed);
		vNewDir =
			Vec3::Lerp(m_pTargetCam->Transform()->GetWorldDir(DIR_TYPE::FRONT), vNewDir, DT * m_tInfo.fCamRotSpeed);
		fCurDistance = RoRMath::Lerp(fCurDistance, m_tInfo.fDistance, DT * m_tInfo.fCamSpeed);
	}

	tRoRHitInfo tHitInfo;

	if (CPhysXMgr::GetInst()->PerfomRaycast(vNewPos, -vNewDir, tHitInfo, (UINT)LAYER::LAYER_RAYCAST,
											(int)RayCastDebugFlag::AllInvisible))
	{
		float length = (tHitInfo.vHitPos - vNewPos).Length();
		SetCurDistance(length);
	}
	else
	{
		SetCurDistance(m_tInfo.fMaxDistance);
	}

	m_pTargetCam->Transform()->SetRelativePos(vNewPos);
	m_pTargetCam->Transform()->SetDir(vNewDir);
}

void CSpringArm::SetTargetCamera(CCamera* _cam)
{
	m_pTargetCam = _cam->GetOwner();
}
