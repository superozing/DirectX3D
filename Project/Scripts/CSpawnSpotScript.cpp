#include "pch.h"
#include "CSpawnSpotScript.h"

#include <Engine\CAssetMgr.h>

CSpawnSpotScript::CSpawnSpotScript()
	: CScript((UINT)SCRIPT_TYPE::SPAWNSPOTSCRIPT)
	, SpawnBasicPosition(0.f, 0.f, 0.f)
	, SpawnBasicScale(0.f, 0.f, 0.f)
	, SpawnObjectPath("")
	, SpawnObjType(SpawnObjType::NONE)
{
	AppendScriptParam("Spawn Position", SCRIPT_PARAM::VEC3, &SpawnBasicPosition);
	AppendScriptParam("Spawn Scale   ", SCRIPT_PARAM::VEC3, &SpawnBasicScale);
	AppendScriptParam("Spawn Object   ", SCRIPT_PARAM::STRING, &SpawnObjectPath);

	// AppendMemberFunction("Change Type", SCRIPT_PARAM::FUNC_MEMBER, "Type",
	//					 [this]() { this->SetSpawnObjType(int iNewType)); });

	// void에 void
}
CSpawnSpotScript::~CSpawnSpotScript()
{
}

void CSpawnSpotScript::SetSpawnObjType(int iNewType)
{
}

void CSpawnSpotScript::begin()
{
	SpawnBasicPosition = GetOwner()->Transform()->GetWorldPos();

	Ptr<CMeshData> pMeshData = CAssetMgr::GetInst()->Load<CMeshData>(ToWString(SpawnObjectPath));

	if (pMeshData != nullptr)
	{
		CGameObject* BindObject = pMeshData->Instantiate();
		BindObject->SetName(pMeshData->GetKey());

		BindObject->Transform()->SetRelativePos(SpawnBasicPosition);
		BindObject->Transform()->SetRelativeScale(SpawnBasicScale);

		Vec3 PointerRotation = GetOwner()->Transform()->GetWorldRot();
		BindObject->Transform()->SetRelativeRotation(PointerRotation);

		GamePlayStatic::SpawnGameObject(BindObject, 0);
	}

	GamePlayStatic::DestroyGameObject(GetOwner());
}

void CSpawnSpotScript::tick()
{
}

void CSpawnSpotScript::SaveToFile(FILE* _File)
{
}

void CSpawnSpotScript::LoadFromFile(FILE* _File)
{
}
