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

void CDamageRay::tick()
{
	tRoRHitInfo hitInfo = {};

	CPhysXMgr::GetInst()->PerfomRaycast(GetOwner()->Transform()->GetWorldPos(),
										GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT), hitInfo,
										(UINT)LAYER::LAYER_PLAYER);
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
