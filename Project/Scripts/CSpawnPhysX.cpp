#include "pch.h"
#include "CSpawnPhysX.h"

#include "Engine\CKeyMgr.h"
#include "CSpawnSpotScript.h"
void CSpawnPhysX::begin()
{
}

void CSpawnPhysX::tick()
{
	auto			  vecscript = GetOwner()->GetScripts();
	CSpawnSpotScript* SSS		= nullptr;

	for (auto& e : vecscript)
	{
		if (e->GetScriptType() == (UINT)SCRIPT_TYPE::SPAWNSPOTSCRIPT)
		{
			SSS = static_cast<CSpawnSpotScript*>(e);
		}
	}

	if (KEY_TAP(KEY::SPACE) && nullptr != SSS)
	{
		SSS->RegisterObject();
		SSS->SpawnObject();
	}
}

CSpawnPhysX::CSpawnPhysX()
	: CScript((UINT)SCRIPT_TYPE::SPAWNPHYSX)
{
}

CSpawnPhysX::~CSpawnPhysX()
{
}
