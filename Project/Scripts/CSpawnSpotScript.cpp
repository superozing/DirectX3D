#include "pch.h"
#include "CSpawnSpotScript.h"

#include <Engine\CAssetMgr.h>
#include <Engine\CMemoryPoolMgr.h>
#include "CMemoryPoolMgrScript.h"

CSpawnSpotScript::CSpawnSpotScript()
	: CScript((UINT)SCRIPT_TYPE::SPAWNSPOTSCRIPT)
	, SpawnBasicPosition(0.f, 0.f, 0.f)
	, SpawnBasicScale(0.f, 0.f, 0.f)
	, SpawnObjectPath("")
	, SpawnObjType(SpawnObjType::None)
	, strDisplayString(ToString(magic_enum::enum_name(SpawnObjType::None)))
	, ModeColor{Vec4(1.f, 1.f, 1.f, 1.f)}
	, m_listSpawnObject()
	, m_BindingPrefab()
{
	AppendScriptParam("Spawn Position", SCRIPT_PARAM::VEC3, &SpawnBasicPosition);
	AppendScriptParam("Spawn Scale   ", SCRIPT_PARAM::VEC3, &SpawnBasicScale);
	AppendScriptObject("Recent Register Object", &RecentRegisterObj, COMPONENT_TYPE::TRANSFORM);

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

	m_BindingPrefab = CAssetMgr::GetInst()->Load<CPrefab>(PREFTempPrefab);
}

CSpawnSpotScript::~CSpawnSpotScript()
{
	vector<CGameObject*> vecChild = GetOwner()->GetChild();

	for (auto i = 0; i < vecChild.size(); ++i)
	{
		vecChild[i]->DisconnectWithParent();
	}

	Delete_List(m_listSpawnObject);
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

	CGameObject* pObj = nullptr;
	pObj =
		CMemoryPoolMgr::GetInst()->GetEX()->GetScript<CMemoryPoolMgrScript>()->PopObject(ToString(PREFTutorialTarget2));

	m_listSpawnObject.push_back(pObj);

	RecentRegisterObj = pObj;

	GetOwner()->AddChild(pObj);
}

void CSpawnSpotScript::DeAllocateObject()
{
	for (auto iter = m_CurrentSpawnObject.begin(); iter != m_CurrentSpawnObject.end(); ++iter)
	{
		CMemoryPoolMgr::GetInst()->GetEX()->GetScript<CMemoryPoolMgrScript>()->PushObject(*iter);
	}

	m_CurrentSpawnObject.clear();
}

void CSpawnSpotScript::SpawnObject()
{
	// spawn count 적용이 필요할듯. 일단 1개먼저 만들기
	// bool 값으로 크기 선택 여부
	// 선택한 obj layer - 원래 있던걸로 동작시키기

	if (m_listSpawnObject.size() == 0)
	{
		MessageBox(nullptr, L"등록된 obj가 없음", L"스폰 불가능", MB_OK);
		return;
	}

	CGameObject* pObj = nullptr;
	pObj			  = m_listSpawnObject.front();
	pObj->DisconnectWithParent();
	m_listSpawnObject.pop_front();
	m_CurrentSpawnObject.push_back(pObj);

	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	pObj->Transform()->SetRelativePos(pos);

	pObj->Transform()->SetRelativeRotation(GetOwner()->Transform()->GetRelativeRotation());
	pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 50.f));

	GamePlayStatic::SpawnGameObject(pObj, 0);
}

void CSpawnSpotScript::begin()
{
	SpawnBasicPosition = GetOwner()->Transform()->GetWorldPos();

	for (auto iter = m_listSpawnObject.begin(); iter != m_listSpawnObject.end(); ++iter)
	{
		(*iter)->Transform()->SetRelativePos(SpawnBasicPosition);
		SpawnBasicPosition.x += 300.f;
		(*iter)->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 300.f));

		GamePlayStatic::SpawnGameObject((*iter), 0);
	}

	ModeColor = Vec4(0.f, 0.f, 0.f, 0.f);

	GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
	GetOwner()->MeshRender()->GetMaterial(1)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
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
