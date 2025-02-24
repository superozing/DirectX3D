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
// static Vec3 RayOrigin = Vec3(0.f, 500.f, 0.f);
// static Vec3 RayDir	  = Vec3(0.f, -1.f, 0.f);

// RayContactInfo
static tRoRHitInfo hitinfo	  = {};
static bool		   iscontact  = false;
static string	   strobj	  = "";
static Vec3		   contactpos = Vec3();
//
// static bool bTypeViewPortRC = false;

void CPhysXMgrScript::begin()
{
	AppendScriptParam("pPhysXMgr", SCRIPT_PARAM::MGR_PHYSX, (void*)CPhysXMgr::GetInst());

	// AppendScriptParam("[Script]RayOrigin", SCRIPT_PARAM::VEC3, (void*)&RayOrigin);
	// AppendScriptParam("[Script]RayDir", SCRIPT_PARAM::VEC3, (void*)&RayDir);

	AppendScriptParam("[Script]raycol_result", SCRIPT_PARAM::BOOL, (void*)&iscontact, 0.f, 0.f, true);
	AppendScriptParam("[Script]raycol_obj", SCRIPT_PARAM::STRING, (void*)&strobj, 0.f, 0.f, true);
	// AppendScriptParam("[Script]raycol_pos", SCRIPT_PARAM::VEC3, (void*)&contactpos, 0.f, 0.f, true);

	// AppendScriptParam("[Script]T:ViewPortRC/F:PointRC", SCRIPT_PARAM::BOOL, (void*)&bTypeViewPortRC);
}

void CPhysXMgrScript::tick()
{
	// if (false == bTypeViewPortRC)
	//{
	//	// 일반 Raycast
	//	int mask  = RayCastDebugFlag::AllVisible;
	//	iscontact = CPhysXMgr::GetInst()->PerfomRaycast(Vec3(RayOrigin.x, RayOrigin.y, RayOrigin.z),
	//													Vec3(RayDir.x, RayDir.y, RayDir.z), hitinfo,
	//													(UINT)LAYER::LAYER_RAYCAST, mask);
	// }
	// else
	//{
	// 뷰포트 Raycast
	// int mask  = RayCastDebugFlag::AllInvisible;
	// iscontact = CPhysXMgr::GetInst()->ViewPortRaycast(hitinfo, mask);
	// }

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
