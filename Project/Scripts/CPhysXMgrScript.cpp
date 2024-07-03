﻿#include "pch.h"
#include "CPhysXMgrScript.h"

#include <Engine\CPhysXMgr.h>

CPhysXMgrScript::CPhysXMgrScript()
	: CScript((UINT)SCRIPT_TYPE::PHYSXMGRSCRIPT)
{
}

CPhysXMgrScript::~CPhysXMgrScript()
{
}

// RayInput
static Vec3 RayOrigin = Vec3(0.f, 500.f, 0.f);
static Vec3 RayDir	  = Vec3(0.f, -1.f, 0.f);

// RayContactInfo
static tRoRHitInfo hitinfo	  = {};
static bool		   iscontact  = false;
static string	   strobj	  = "";
static Vec3		   contactpos = Vec3();

static bool bStart	 = false;
static bool bEnd	 = false;
static bool bRayLine = false;

void CPhysXMgrScript::begin()
{
	AppendScriptParam("pPhysXMgr", SCRIPT_PARAM::MGR_PHYSX, (void*)CPhysXMgr::GetInst());

	AppendScriptParam("[Script]RayOrigin", SCRIPT_PARAM::VEC3, (void*)&RayOrigin);
	AppendScriptParam("[Script]RayDir", SCRIPT_PARAM::VEC3, (void*)&RayDir);

	AppendScriptParam("[Script]raycol_result", SCRIPT_PARAM::BOOL, (void*)&iscontact, 0.f, 0.f, true);
	AppendScriptParam("[Script]raycol_obj", SCRIPT_PARAM::STRING, (void*)&strobj, 0.f, 0.f, true);
	AppendScriptParam("[Script]raycol_pos", SCRIPT_PARAM::VEC3, (void*)&contactpos, 0.f, 0.f, true);

	AppendScriptParam("[Script]Debug StartPoint", SCRIPT_PARAM::BOOL, (void*)&bStart);
	AppendScriptParam("[Script]Debug StartEnd", SCRIPT_PARAM::BOOL, (void*)&bEnd);
	AppendScriptParam("[Script]Debug StartEnd", SCRIPT_PARAM::BOOL, (void*)&bRayLine);
}

void CPhysXMgrScript::tick()
{
	int mask = 0;
	if (true == bStart)
	{
		mask = mask | RayCastDebugFlag::StartPointVisible;
	}
	if (true == bEnd)
	{
		mask = mask | RayCastDebugFlag::EndPointVisible;
	}
	if (true == bRayLine)
	{
		mask = mask | RayCastDebugFlag::RayLineVisible;
	}

	// 일반 Raycast
	iscontact = CPhysXMgr::GetInst()->PerfomRaycast(Vec3(RayOrigin.x, RayOrigin.y, RayOrigin.z),
													Vec3(RayDir.x, RayDir.y, RayDir.z), hitinfo,
													(UINT)LAYER::LAYER_RAYCAST, mask);

	if (true == iscontact)
	{
		strobj	   = ToString(hitinfo.pOtherObj->GetName());
		contactpos = hitinfo.vHitPos;
	}
	else
	{
		strobj = "";
	}

	// 뷰포트 Raycast
	// iscontact = CPhysXMgr::GetInst()->ViewPortRaycast(hitinfo);

	// if (true == iscontact)
	//{
	//	strobj	   = ToString(hitinfo.pOtherObj->GetName());
	//	contactpos = hitinfo.vHitPos;
	// }
	// else
	//{
	//	strobj = "";
	// }
}
