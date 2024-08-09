#include "pch.h"
#include "CDamageRay.h"

#include <Engine\CPhysXMgr.h>
#include "CPlayerScript.h"

#define RayDamage 30.f

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

	bool bHit = false;
	bHit	  = CPhysXMgr::GetInst()->PerformRaycast(RayPos, WorldFrontDir, hitInfo, (UINT)LAYER::LAYER_MONSTER_SKILL,
													 RayCastDebugFlag::AllVisible);

	if (bHit)
	{
		if (hitInfo.pOtherObj->GetName() == L"Azusa")
		{
			CPlayerScript* pScript = nullptr;
			pScript				   = hitInfo.pOtherObj->GetScript<CPlayerScript>();

			if (pScript != nullptr)
			{
				pScript->SetDamagedMove(true);
				pScript->Hit(RayDamage);
			}
		}
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
