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
