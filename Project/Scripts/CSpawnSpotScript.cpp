#include "pch.h"
#include "CSpawnSpotScript.h"

#include <Engine\CAssetMgr.h>
#include <Engine\CMemoryPoolMgr.h>

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
	m_vecSpawnObject.reserve(1);

	AppendScriptParam("Spawn Position", SCRIPT_PARAM::VEC3, &SpawnBasicPosition);
	AppendScriptParam("Spawn Scale   ", SCRIPT_PARAM::VEC3, &SpawnBasicScale);
	AppendScriptObject("Spawn Obj", &CurObjectPointer, COMPONENT_TYPE::TRANSFORM);

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
	AppendMemberFunction("Deallocate", SCRIPT_PARAM::FUNC_MEMBER, "DeAlloctae",
						 std::bind(&CSpawnSpotScript::DeAllocateObject, this));

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

#include <Engine\CLogMgr.h>
void CSpawnSpotScript::RegisterObject()
{
	CGameObject* pObj = nullptr;
	pObj			  = CMemoryPoolMgr::GetInst()->Allocate();

	m_vecSpawnObject.push_back(pObj);

	CurObjectPointer = pObj;

	// string s = std::to_string((int)m_vecSpawnObject.size());
	// CLogMgr::GetInst()->AddLog(Log_Level::INFO, s);
}

void CSpawnSpotScript::DeAllocateObject()
{
	CMemoryPoolMgr::GetInst()->DeAllocate(m_CurrentSpawnObject.back());
	m_CurrentSpawnObject.pop_back();
}

void CSpawnSpotScript::SpawnObject()
{
	CGameObject* pObj = nullptr;
	pObj			  = m_vecSpawnObject.back();
	m_vecSpawnObject.pop_back();
	m_CurrentSpawnObject.push_back(pObj);

	pObj->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	pObj->Transform()->SetRelativeRotation(GetOwner()->Transform()->GetRelativeRotation());
	pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 50.f));

	GamePlayStatic::SpawnGameObject(pObj, 0);

	pObj = nullptr;
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
