#include "pch.h"
#include "CSpawnSpotScript.h"

#include <Engine\CAssetMgr.h>

CSpawnSpotScript::CSpawnSpotScript()
	: CScript((UINT)SCRIPT_TYPE::SPAWNSPOTSCRIPT)
	, SpawnBasicPosition(0.f, 0.f, 0.f)
	, SpawnBasicScale(0.f, 0.f, 0.f)
	, SpawnObjectPath("")
	, SpawnObjType(SpawnObjType::None)
	, strDisplayString(ToString(magic_enum::enum_name(SpawnObjType::None)))
	, ModeColor{Vec4(1.f, 1.f, 1.f, 1.f)}
{
	AppendScriptParam("Spawn Position", SCRIPT_PARAM::VEC3, &SpawnBasicPosition);
	AppendScriptParam("Spawn Scale   ", SCRIPT_PARAM::VEC3, &SpawnBasicScale);
	AppendScriptParam("Spawn Object   ", SCRIPT_PARAM::STRING, &SpawnObjectPath);

	AppendScriptParam("SPOT TYPE", SCRIPT_PARAM::STRING, &strDisplayString, 0.f, 0.f, true);

	AppendMemberFunction("SpawnTypePlayer", SCRIPT_PARAM::FUNC_MEMBER, "PlayerType",
						 std::bind(&CSpawnSpotScript::SetSpawnTypePlayer, this));

	AppendMemberFunction("SpawnTypeMonster", SCRIPT_PARAM::FUNC_MEMBER, "MonsterType",
						 std::bind(&CSpawnSpotScript::SetSpawnTypeMonster, this));

	AppendMemberFunction("SpawnTypeBoss", SCRIPT_PARAM::FUNC_MEMBER, "BossType",
						 std::bind(&CSpawnSpotScript::SetSpawnTypeBoss, this));

	AppendMemberFunction("SpawnTypeEtc", SCRIPT_PARAM::FUNC_MEMBER, "EtcType",
						 std::bind(&CSpawnSpotScript::SetSpawnTypeETC, this));

	AppendMemberFunction("SpawnTypeNone", SCRIPT_PARAM::FUNC_MEMBER, "NoneType",
						 std::bind(&CSpawnSpotScript::SetSpawnTypeNone, this));
}
CSpawnSpotScript::~CSpawnSpotScript()
{
}

void CSpawnSpotScript::SetSpawnTypePlayer()
{
	SpawnObjType	 = SpawnObjType::Player;
	strDisplayString = ToString(magic_enum::enum_name(SpawnObjType::Player));
	ModeColor		 = Vec4(0.f, 1.f, 0.f, 1.f);

	GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
	GetOwner()->MeshRender()->GetMaterial(1)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
}

void CSpawnSpotScript::SetSpawnTypeMonster()
{
	SpawnObjType	 = SpawnObjType::Monster;
	strDisplayString = ToString(magic_enum::enum_name(SpawnObjType::Monster));
	ModeColor		 = Vec4(1.f, 0.f, 0.f, 1.f);

	GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
	GetOwner()->MeshRender()->GetMaterial(1)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
}

void CSpawnSpotScript::SetSpawnTypeBoss()
{
	SpawnObjType	 = SpawnObjType::BOSS;
	strDisplayString = ToString(magic_enum::enum_name(SpawnObjType::BOSS));
	ModeColor		 = Vec4(0.f, 0.f, 1.f, 1.f);

	GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
	GetOwner()->MeshRender()->GetMaterial(1)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
}

void CSpawnSpotScript::SetSpawnTypeETC()
{
	SpawnObjType	 = SpawnObjType::Etc;
	strDisplayString = ToString(magic_enum::enum_name(SpawnObjType::Etc));
	ModeColor		 = Vec4(0.3f, 0.3f, 0.3f, 1.f);

	GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
	GetOwner()->MeshRender()->GetMaterial(1)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
}

void CSpawnSpotScript::SetSpawnTypeNone()
{
	SpawnObjType	 = SpawnObjType::None;
	strDisplayString = ToString(magic_enum::enum_name(SpawnObjType::None));
	ModeColor		 = Vec4(0.f, 0.f, 0.f, 1.f);

	GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
	GetOwner()->MeshRender()->GetMaterial(1)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
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
