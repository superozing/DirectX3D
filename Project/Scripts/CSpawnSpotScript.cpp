﻿#include "pch.h"
#include "CSpawnSpotScript.h"

#include <Engine\CAssetMgr.h>
#include <Engine\CMemoryPoolMgr.h>
#include "CMemoryPoolMgrScript.h"

CSpawnSpotScript::CSpawnSpotScript()
	: CScript((UINT)SCRIPT_TYPE::SPAWNSPOTSCRIPT)
	, SpawnBasicPosition(0.f, 0.f, 0.f)
	, SpawnType(SpawnObjType::None)
	, m_iBindObjCount(0)
	, strDisplaySpawnTypeString(ToString(magic_enum::enum_name(SpawnObjType::None)))
	, ModeColor{Vec4(1.f, 1.f, 1.f, 1.f)}
	, m_listSpawnObject()
	, m_vecPrefabKey()
{
	AppendScriptParam("Spawn Position", SCRIPT_PARAM::VEC3, &SpawnBasicPosition);
	AppendScriptParam("BindObj Count", SCRIPT_PARAM::INT, &m_iBindObjCount, 0, 0, true);

	// APPEND SCRIPT VECTOR로 변경할 수도
	for (int i = 0; i < m_vecPrefabKey.size(); ++i)
	{
		AppendScriptParam("Prefabkey" + std::to_string(i), SCRIPT_PARAM::STRING, &m_vecPrefabKey[i], 0, 0, true, {});
	}

	AppendScriptParam("SPOT TYPE", SCRIPT_PARAM::STRING, &strDisplaySpawnTypeString, 0.f, 0.f, true);

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
	vector<CGameObject*> vecChild = GetOwner()->GetChild();

	for (auto i = 0; i < vecChild.size(); ++i)
	{
		vecChild[i]->DisconnectWithParent();
	}

	Delete_List(m_listSpawnObject);
}

void CSpawnSpotScript::SetSpawnTypePlayer()
{
	SpawnType				  = SpawnObjType::Player;
	strDisplaySpawnTypeString = ToString(magic_enum::enum_name(SpawnObjType::Player));
	ModeColor				  = Vec4(0.f, 1.f, 0.f, 1.f);

	GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
	GetOwner()->MeshRender()->GetMaterial(1)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
}

void CSpawnSpotScript::SetSpawnTypeMonster()
{
	SpawnType				  = SpawnObjType::Monster;
	strDisplaySpawnTypeString = ToString(magic_enum::enum_name(SpawnObjType::Monster));
	ModeColor				  = Vec4(1.f, 0.f, 0.f, 1.f);

	GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
	GetOwner()->MeshRender()->GetMaterial(1)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
}

void CSpawnSpotScript::SetSpawnTypeBoss()
{
	SpawnType				  = SpawnObjType::BOSS;
	strDisplaySpawnTypeString = ToString(magic_enum::enum_name(SpawnObjType::BOSS));
	ModeColor				  = Vec4(0.f, 0.f, 1.f, 1.f);

	GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
	GetOwner()->MeshRender()->GetMaterial(1)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
}

void CSpawnSpotScript::SetSpawnTypeETC()
{
	SpawnType				  = SpawnObjType::Etc;
	strDisplaySpawnTypeString = ToString(magic_enum::enum_name(SpawnObjType::Etc));
	ModeColor				  = Vec4(0.3f, 0.3f, 0.3f, 1.f);

	GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
	GetOwner()->MeshRender()->GetMaterial(1)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
}

void CSpawnSpotScript::SetSpawnTypeNone()
{
	SpawnType				  = SpawnObjType::None;
	strDisplaySpawnTypeString = ToString(magic_enum::enum_name(SpawnObjType::None));
	ModeColor				  = Vec4(0.f, 0.f, 0.f, 1.f);

	GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
	GetOwner()->MeshRender()->GetMaterial(1)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
}

void CSpawnSpotScript::RegisterObject()
{

	CGameObject* pObj = nullptr;
	pObj =
		CMemoryPoolMgr::GetInst()->GetEX()->GetScript<CMemoryPoolMgrScript>()->PopObject(ToString(PREFTutorialTarget2));

	m_listSpawnObject.push_back(pObj);

	m_iBindObjCount = m_listSpawnObject.size();

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

	m_iBindObjCount = m_listSpawnObject.size();
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

#define TagSpawnType "[SpawnType]"
#define TagPrefabCount "[PrefabCount]"
#define TagPrefabKey "[PrefabKey]"

void CSpawnSpotScript::SaveToFile(ofstream& fout)
{
	fout << TagSpawnType << endl;
	fout << magic_enum::enum_name(SpawnType) << endl;

	fout << TagPrefabCount << endl;
	fout << (int)m_vecPrefabKey.size() << endl;

	for (int i = 0; i < m_vecPrefabKey.size(); ++i)
	{
		fout << TagPrefabKey << endl;
		fout << m_vecPrefabKey[i] << endl;
	}
}

void CSpawnSpotScript::LoadFromFile(ifstream& fin)
{
	string strSpawnType;

	Utils::GetLineUntilString(fin, TagSpawnType);
	getline(fin, strSpawnType);

	auto Type = magic_enum::enum_cast<SpawnObjType>(strSpawnType);
	SpawnType = Type.value();

	int iPrefabCount;
	Utils::GetLineUntilString(fin, TagPrefabCount);
	fin >> iPrefabCount;

	for (int i = 0; i < iPrefabCount; ++i)
	{
		string s;
		Utils::GetLineUntilString(fin, TagPrefabKey);
		fin >> s;

		m_vecPrefabKey.push_back(s);
	}

	for (int i = 0; i < m_vecPrefabKey.size(); ++i)
	{
		CMemoryPoolMgr::GetInst()->Poolbegin(ToString(m_vecPrefabKey[i]));
	}
}
