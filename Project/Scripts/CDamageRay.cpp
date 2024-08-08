#include "pch.h"
#include "CDamageRay.h"

#include <Engine\CPhysXMgr.h>

CDamageRay::CDamageRay()
	: CScript((UINT)SCRIPT_TYPE::DAMAGERAY)
{
}

CDamageRay::CDamageRay(const CDamageRay& _Other)
	: CScript((UINT)_Other.GetScriptType())
{
}

CDamageRay::~CDamageRay()
{
}

void CDamageRay::begin()
{
}

#include <Engine\CLogMgr.h>
void CDamageRay::tick()
{
	tRoRHitInfo hitInfo = {};

	Vec3 RayPos		   = GetOwner()->Transform()->GetWorldPos();
	Vec3 WorldFrontDir = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT);

	bool bHit = CPhysXMgr::GetInst()->PerfomRaycast(RayPos, WorldFrontDir, hitInfo, (UINT)LAYER::LAYER_PLAYER,
													RayCastDebugFlag::AllVisible);

	if (bHit)
	{
		string s = "Player Hit";
		CLogMgr::GetInst()->AddLog(Log_Level::INFO, s);
	}
}

void CDamageRay::SaveToFile(FILE* _File)
{
}

void CDamageRay::SaveToFile(ofstream& fout)
{
}

void CDamageRay::LoadFromFile(FILE* _File)
{
}

void CDamageRay::LoadFromFile(ifstream& fin)
{
}
