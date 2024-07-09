﻿#include "pch.h"
#include "CSpringArm.h"

#include <Engine\CRenderMgr.h>
#include <Engine/CPhysXMgr.h>

static Vec3 gDir = Vec3();
CSpringArm::CSpringArm()
	: CScript((UINT)SCRIPT_TYPE::SPRINGARM)
	, m_tInfo{}

{
	AppendScriptParam("Active", SCRIPT_PARAM::BOOL, &m_bActive);
	AppendScriptParam("Smooth", SCRIPT_PARAM::BOOL, &m_tInfo.Type);
	AppendScriptParam("Distance", SCRIPT_PARAM::FLOAT, &m_tInfo.fMaxDistance);
	AppendScriptParam("Speed", SCRIPT_PARAM::FLOAT, &m_tInfo.fCamSpeed);
	AppendScriptParam("RotSpeed", SCRIPT_PARAM::FLOAT, &m_tInfo.fCamRotSpeed);
	AppendScriptParam("Direction", SCRIPT_PARAM::VEC3, &m_tInfo.vDir);

	AppendScriptObject("Cam", &m_pTarget, COMPONENT_TYPE::TRANSFORM);
}

CSpringArm::~CSpringArm()
{
}

void CSpringArm::begin()
{
}

void CSpringArm::tick()
{
	if (!GetOwner() || !Transform() || !m_pTarget || !m_bActive)
		return;

	static float fCurDistance = m_tInfo.fDistance;

	Matrix worldMat = Transform()->GetWorldMat();

	gDir = XMVector3TransformNormal(m_tInfo.vDir, worldMat);

	Vec3 vCenterPos = Transform()->GetWorldPos();
	Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();

	Vec3 vNewPos = vCenterPos + gDir * m_tInfo.fDistance;
	Vec3 vNewDir = vCenterPos - vTargetPos;

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

	m_pTarget->Transform()->SetRelativePos(vNewPos);
	m_pTarget->Transform()->SetDir(m_vDirOffset + vNewDir);
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
	Utils::GetLineUntilString(fin, TagSpringArm);
	fin >> m_tInfo;

	Utils::GetLineUntilString(fin, TagActive);
	fin >> m_bActive;
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
