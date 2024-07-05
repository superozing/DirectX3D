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
	, m_vecSpawnObject()
{
	m_vecSpawnObject.resize(1);

	AppendScriptParam("Spawn Position", SCRIPT_PARAM::VEC3, &SpawnBasicPosition);
	AppendScriptParam("Spawn Scale   ", SCRIPT_PARAM::VEC3, &SpawnBasicScale);
	AppendScriptObject("Spawn Obj", &(m_vecSpawnObject[0]), COMPONENT_TYPE::TRANSFORM);

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

	AppendMemberFunction("Spawn", SCRIPT_PARAM::FUNC_MEMBER, "Spawn", std::bind(&CSpawnSpotScript::SpawnObject, this));

	AppendMemberFunction("Register", SCRIPT_PARAM::FUNC_MEMBER, "Register",
						 std::bind(&CSpawnSpotScript::RegisterObject, this));
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

void CSpawnSpotScript::RegisterObject()
{
	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"Sphere");

	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
	pObj->Transform()->SetDir(Vec3(0.f, -1.f, 0.f));

	pObj->AddComponent(new CMeshRender);
	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHsphere));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std3D_DeferredMtrl"), 0);
	pObj->MeshRender()->GetMaterial(0)->SetTexParam(
		TEX_PARAM::TEX_0,
		CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01.tga", L"texture\\tile\\TILE_01.tga"));
	pObj->MeshRender()->GetMaterial(0)->SetTexParam(
		TEX_PARAM::TEX_1,
		CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01_N.tga", L"texture\\tile\\TILE_01_N.tga"));

	pObj->AddComponent(new CPhysX);
	pObj->PhysX()->m_bStaticActor = false;
	pObj->PhysX()->m_Shape		  = PhysShape::SPHERE;

	m_vecSpawnObject[0] = pObj;
}

void CSpawnSpotScript::SpawnObject()
{
	m_vecSpawnObject[0]->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	m_vecSpawnObject[0]->Transform()->SetRelativeRotation(GetOwner()->Transform()->GetRelativeRotation());

	GamePlayStatic::SpawnGameObject(m_vecSpawnObject[0], 0);

	m_vecSpawnObject[0] = nullptr;
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
