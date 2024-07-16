#include "pch.h"
#include "CSpawnSpotScript.h"

#include <Engine\CAssetMgr.h>
#include <Engine\CMemoryPoolMgr.h>
#include "CMemoryPoolMgrScript.h"

CSpawnSpotScript::CSpawnSpotScript()
	: CScript((UINT)SCRIPT_TYPE::SPAWNSPOTSCRIPT)
	, SpawnType(SpawnObjType::None)
	, strDisplaySpawnTypeString(ToString(magic_enum::enum_name(SpawnObjType::None)))
	, ModeColor{Vec4(1.f, 1.f, 1.f, 1.f)}
	, m_listSpawnObject()
	, m_vecPrefabKey()
	, m_ivecModeIdx(0)
	, m_ivecPrefabIdx(0)
	, m_CurBindPrefab(nullptr)
	, m_vecDisplayMode()
{

	//=================
	// Debug용 Type
	//=================

	AppendScriptParam("SPOT TYPE", SCRIPT_PARAM::STRING, &strDisplaySpawnTypeString, 0.f, 0.f, true);

	m_vecDisplayMode.push_back("None");
	m_vecDisplayMode.push_back("Player");
	m_vecDisplayMode.push_back("Monster");
	m_vecDisplayMode.push_back("Boss");
	m_vecDisplayMode.push_back("Etc");

	AppendScriptVector("Spawn Mode", &m_vecDisplayMode, &m_ivecModeIdx, true, "Display Pin");
	SameLine();
	AppendMemberFunction("SetType", SCRIPT_PARAM::FUNC_MEMBER, "Change Pin Color",
						 std::bind(&CSpawnSpotScript::SetDisplayMode, this));

	AppendSeperateLine();
	//=====================
	// Spawn 관련
	//=====================

	// Register를 해야 script의 spawn list에 담겨서 spawn을 할 수 있다.

	AppendMemberFunction("Register", SCRIPT_PARAM::FUNC_MEMBER, "Register",
						 std::bind(&CSpawnSpotScript::RegisterObject, this));

	SameLine();

	AppendMemberFunction("Spawn", SCRIPT_PARAM::FUNC_MEMBER, "Spawn", std::bind(&CSpawnSpotScript::SpawnObject, this));

	SameLine();

	// Spawn되어있는 것들을 다시 풀로 보내는 함수. 디버그용 , spawn 된 물체들을 뺄 수 있다.
	AppendMemberFunction("Deallocate", SCRIPT_PARAM::FUNC_MEMBER, "DeAlloctae",
						 std::bind(&CSpawnSpotScript::DeAllocateAllObject, this));

	AppendSeperateLine();

	//=====================
	// 세팅 관련
	//=====================
	AppendScriptVector("Prefab Vector", &m_vecPrefabKey, &m_ivecPrefabIdx, true,
					   "선택한 idx에 Prefab Pointer와 상호작용 가능");

	AppendScriptAsset("Prefab Pointer", &m_CurBindPrefab, ASSET_TYPE::PREFAB, false, "vector idx current prefab");

	// 현재 idx에 있는 prefab을 빼고 해당 자리에 등록
	AppendMemberFunction("Modify Prefab", SCRIPT_PARAM::FUNC_MEMBER, "RegisterPrefab",
						 std::bind(&CSpawnSpotScript::RegisterPrefab, this));

	SameLine();

	// 새로운 prefab을 추가로 바인딩하는 것
	AppendMemberFunction("Add Prefab", SCRIPT_PARAM::FUNC_MEMBER, "PushBackPrefab",
						 std::bind(&CSpawnSpotScript::PushBackPrefab, this));

	// 해당 script에 할당된 모든 prefab을 지우는 것. + List 내 obj도 지워진다.
	AppendMemberFunction("Clear All", SCRIPT_PARAM::FUNC_MEMBER, "ClearPrefab",
						 std::bind(&CSpawnSpotScript::ClearPrefab, this));

	SameLine();

	// idx를 선택하고 지우면 해당 prefab과 obj만 지워진다.
	AppendMemberFunction("Delete Prefab", SCRIPT_PARAM::FUNC_MEMBER, "DeletePrefab",
						 std::bind(&CSpawnSpotScript::DeletePrefab, this));
}

CSpawnSpotScript::CSpawnSpotScript(const CSpawnSpotScript& _Origin)
	: CScript((UINT)SCRIPT_TYPE::SPAWNSPOTSCRIPT)
	, SpawnType(_Origin.SpawnType)
	, strDisplaySpawnTypeString(_Origin.strDisplaySpawnTypeString)
	, m_vecDisplayMode(_Origin.m_vecDisplayMode)
	, m_ivecModeIdx(_Origin.m_ivecModeIdx)
	, ModeColor(_Origin.ModeColor)
	, m_CurrentSpawnObject()
	, m_vecPrefabKey(_Origin.m_vecPrefabKey)
	, m_CurBindPrefab(nullptr)
	, m_vecPrefab(_Origin.m_vecPrefab)
	, m_ivecPrefabIdx(_Origin.m_ivecPrefabIdx)
{
	for (auto iter = _Origin.m_listSpawnObject.begin(); iter != _Origin.m_listSpawnObject.end(); ++iter)
	{

		CGameObject* pObj	 = *iter;
		CGameObject* CopyObj = pObj->Clone();
		m_listSpawnObject.push_back(CopyObj);
	}

	//=================
	// Debug용 Type
	//=================

	AppendScriptParam("SPOT TYPE", SCRIPT_PARAM::STRING, &strDisplaySpawnTypeString, 0.f, 0.f, true);

	m_vecDisplayMode.push_back("None");
	m_vecDisplayMode.push_back("Player");
	m_vecDisplayMode.push_back("Monster");
	m_vecDisplayMode.push_back("Boss");
	m_vecDisplayMode.push_back("Etc");

	AppendScriptVector("Spawn Mode", &m_vecDisplayMode, &m_ivecModeIdx, true, "Display Pin");
	SameLine();
	AppendMemberFunction("SetType", SCRIPT_PARAM::FUNC_MEMBER, "Change Pin Color",
						 std::bind(&CSpawnSpotScript::SetDisplayMode, this));

	AppendSeperateLine();
	//=====================
	// Spawn 관련
	//=====================

	// Register를 해야 script의 spawn list에 담겨서 spawn을 할 수 있다.

	AppendMemberFunction("Register", SCRIPT_PARAM::FUNC_MEMBER, "Register",
						 std::bind(&CSpawnSpotScript::RegisterObject, this));

	SameLine();

	AppendMemberFunction("Spawn", SCRIPT_PARAM::FUNC_MEMBER, "Spawn", std::bind(&CSpawnSpotScript::SpawnObject, this));

	SameLine();

	// Spawn되어있는 것들을 다시 풀로 보내는 함수. 디버그용 , spawn 된 물체들을 뺄 수 있다.
	AppendMemberFunction("Deallocate", SCRIPT_PARAM::FUNC_MEMBER, "DeAlloctae",
						 std::bind(&CSpawnSpotScript::DeAllocateAllObject, this));

	AppendSeperateLine();

	//=====================
	// 세팅 관련
	//=====================
	AppendScriptVector("Prefab Vector", &m_vecPrefabKey, &m_ivecPrefabIdx, true,
					   "선택한 idx에 Prefab Pointer와 상호작용 가능");

	AppendScriptAsset("Prefab Pointer", &m_CurBindPrefab, ASSET_TYPE::PREFAB, false, "vector idx current prefab");

	// 현재 idx에 있는 prefab을 빼고 해당 자리에 등록
	AppendMemberFunction("Modify Prefab", SCRIPT_PARAM::FUNC_MEMBER, "RegisterPrefab",
						 std::bind(&CSpawnSpotScript::RegisterPrefab, this));

	SameLine();

	// 새로운 prefab을 추가로 바인딩하는 것
	AppendMemberFunction("Add Prefab", SCRIPT_PARAM::FUNC_MEMBER, "PushBackPrefab",
						 std::bind(&CSpawnSpotScript::PushBackPrefab, this));

	// 해당 script에 할당된 모든 prefab을 지우는 것. + List 내 obj도 지워진다.
	AppendMemberFunction("Clear All", SCRIPT_PARAM::FUNC_MEMBER, "ClearPrefab",
						 std::bind(&CSpawnSpotScript::ClearPrefab, this));

	SameLine();

	// idx를 선택하고 지우면 해당 prefab과 obj만 지워진다.
	AppendMemberFunction("Delete Prefab", SCRIPT_PARAM::FUNC_MEMBER, "DeletePrefab",
						 std::bind(&CSpawnSpotScript::DeletePrefab, this));
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

void CSpawnSpotScript::RegisterObject()
{
	// Pool 역할을 하는 EX로부터 REGISTER
	CGameObject* pObj = nullptr;

	if (m_vecPrefabKey.size() <= 0)
	{
		MessageBox(nullptr, L"프리팹 등록이 필요합니다.", L"SpawnSpotScript", 0);
		return;
	}
	string key = m_vecPrefabKey[m_ivecPrefabIdx];

	pObj = CMemoryPoolMgr::GetInst()->GetEX()->GetScript<CMemoryPoolMgrScript>()->PopObject(key);

	m_listSpawnObject.push_back(pObj);

	GetOwner()->AddChild(pObj, true);
}

void CSpawnSpotScript::DeAllocateAllObject()
{
	for (auto iter = m_CurrentSpawnObject.begin(); iter != m_CurrentSpawnObject.end(); ++iter)
	{
		CMemoryPoolMgr::GetInst()->GetEX()->GetScript<CMemoryPoolMgrScript>()->PushObject(*iter);
	}

	m_CurrentSpawnObject.clear();
}

void CSpawnSpotScript::SpawnObject()
{
	if (m_listSpawnObject.size() == 0)
	{
		MessageBox(nullptr, L"등록된 obj가 없음", L"스폰 불가능", MB_OK);
		return;
	}

	// 등록된 순서대로 나갈 수 있도록 한다.
	CGameObject* pObj = nullptr;
	pObj			  = m_listSpawnObject.front();
	pObj->DisconnectWithParent();
	m_listSpawnObject.pop_front();
	m_CurrentSpawnObject.push_back(pObj);

	// Spot Obj의 위치, 회전 설정
	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	pObj->Transform()->SetRelativePos(pos);

	pObj->Transform()->SetRelativeRotation(GetOwner()->Transform()->GetRelativeRotation());
	pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 50.f));

	string pObjName	 = ToString(pObj->GetName());
	int	   Prefabidx = -1;

	Prefabidx = CMemoryPoolMgr::GetInst()->GetPrefabLayer(pObj);

	GamePlayStatic::SpawnGameObject(pObj, Prefabidx);
}

void CSpawnSpotScript::RegisterPrefab()
{
	for (int i = 0; i < m_vecPrefab.size(); ++i)
	{
		if (m_vecPrefab[i] == m_CurBindPrefab)
		{
			MessageBox(nullptr, L"이미 등록한 Prefab", L"같은 prefab이다", MB_OK);
			return;
		}
	}

	if (m_CurBindPrefab != nullptr)
	{

		if (m_vecPrefabKey.size() <= 0)
		{
			PushBackPrefab();
		}
		else
		{
			// Prefab vector 와 PrefabKey vector 갱신
			m_vecPrefab[m_ivecPrefabIdx] = m_CurBindPrefab;

			string PrevstrPrefabKey			= m_vecPrefabKey[m_ivecPrefabIdx];
			m_vecPrefabKey[m_ivecPrefabIdx] = ToString(m_vecPrefab[m_ivecPrefabIdx]->GetKey());

			// 기존에 bind된 prefab으로 등록된 obj 삭제
			std::list<CGameObject*>::iterator iter = m_listSpawnObject.begin();
			while (iter != m_listSpawnObject.end())
			{
				CGameObject* pObj	 = *iter;
				string		 strFind = ToString(pObj->GetName());

				if (strFind.find(PrevstrPrefabKey))
				{
					pObj->DisconnectWithParent();

					// Pushobj
					CMemoryPoolMgr::GetInst()->PushObject(PrevstrPrefabKey, pObj);

					// iterator 삭제
					iter = m_listSpawnObject.erase(iter); // erase가 iterator를 반환함

					CTaskMgr::GetInst()->SetMemoryPoolEvent(true);
				}
				else
				{
					++iter;
				}
			}
		}
	}
}

void CSpawnSpotScript::PushBackPrefab()
{
	for (int i = 0; i < m_vecPrefab.size(); ++i)
	{
		if (m_vecPrefab[i] == m_CurBindPrefab)
		{
			MessageBox(nullptr, L"이미 등록한 Prefab", L"존재하는 prefab", MB_OK);
			return;
		}
	}

	if (m_CurBindPrefab != nullptr)
	{
		string strPrefabKey = ToString(m_CurBindPrefab->GetKey());

		m_vecPrefab.push_back(m_CurBindPrefab);
		m_vecPrefabKey.push_back(strPrefabKey);

		// 선택된 idx를 추가한걸로 바꾼다.
		if (m_vecPrefab.size() <= 0)
			m_ivecPrefabIdx = 0;
		else
			m_ivecPrefabIdx = m_vecPrefab.size() - 1;
	}
}

void CSpawnSpotScript::ClearPrefab()
{
	for (int i = 0; i < m_vecPrefabKey.size(); ++i)
	{
		std::list<CGameObject*>::iterator iter = m_listSpawnObject.begin();
		while (iter != m_listSpawnObject.end())
		{
			CGameObject* pObj	 = *iter;
			string		 strFind = ToString(pObj->GetName());

			pObj->DisconnectWithParent();

			// 요소를 삭제하기 전에 처리
			CMemoryPoolMgr::GetInst()->PushObject(m_vecPrefabKey[i], pObj);

			// iterator 삭제
			iter = m_listSpawnObject.erase(iter);
		}
	}

	CTaskMgr::GetInst()->SetMemoryPoolEvent(true);

	m_vecPrefab.clear();
	m_vecPrefabKey.clear();
}

void CSpawnSpotScript::DeletePrefab()
{
	if (m_vecPrefab.size() == 0)
	{
		MessageBox(nullptr, L"프리팹 벡터의 사이즈가 0 입니다", L"삭제 불가능", MB_OK);
		return;
	}

	if (m_vecPrefab.size() == 1)
	{
		ClearPrefab();
	}
	else
	{
		string DelPrefab = m_vecPrefabKey[m_ivecPrefabIdx];

		std::list<CGameObject*>::iterator iter = m_listSpawnObject.begin();
		while (iter != m_listSpawnObject.end())
		{
			CGameObject* pObj	 = *iter;
			string		 strFind = ToString(pObj->GetName());

			if (strFind.find(DelPrefab))
			{
				pObj->DisconnectWithParent();

				// 요소를 삭제하기 전에 처리
				CMemoryPoolMgr::GetInst()->PushObject(DelPrefab, pObj);

				// 요소를 삭제하고, 다음 iterator를 얻기 위해 erase 메서드 사용
				iter = m_listSpawnObject.erase(iter); // erase가 iterator를 반환함
			}
			else
			{
				++iter; // 현재 요소를 삭제하지 않은 경우, iterator를 증가시킵니다
			}
		}

		m_vecPrefab.erase(m_vecPrefab.begin() + m_ivecPrefabIdx);
		m_vecPrefabKey.erase(m_vecPrefabKey.begin() + m_ivecPrefabIdx);

		if (m_ivecPrefabIdx >= m_vecPrefab.size() && m_ivecPrefabIdx != 0)
			--m_ivecPrefabIdx;
	}
}

void CSpawnSpotScript::begin()
{
	// 이벤트로 스폰할 예정이니 begin은 할땐 투명처리만
	ModeColor = Vec4(0.f, 0.f, 0.f, 0.f);

	GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
	GetOwner()->MeshRender()->GetMaterial(1)->SetScalarParam(SCALAR_PARAM::VEC4_0, ModeColor);
	m_vecPrefabKey.resize(1);
}

#include <Engine\CKeyMgr.h>
void CSpawnSpotScript::tick()
{
	if (KEY_TAP(KEY::H))
	{
		if (m_CurrentSpawnObject.size() > 0)
		{
			CGameObject* pObj = m_CurrentSpawnObject.front();
			m_CurrentSpawnObject.pop_front();
			CMemoryPoolMgr::GetInst()->PushObject(pObj);
		}
	}
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
		Ptr<CPrefab> pPrefab = CAssetMgr::GetInst()->Load<CPrefab>(m_vecPrefabKey[i]);

		m_vecPrefab.push_back(pPrefab);
	}

	for (int i = 0; i < m_vecPrefabKey.size(); ++i)
	{
		CMemoryPoolMgr::GetInst()->Poolbegin(ToString(m_vecPrefabKey[i]));
	}
}

void CSpawnSpotScript::SetDisplayMode()
{
	switch (m_ivecModeIdx)
	{
	case 0: {
		SetSpawnTypeNone();
	}
	break;
	case 1: {
		SetSpawnTypePlayer();
	}
	break;
	case 2: {
		SetSpawnTypeMonster();
	}
	break;
	case 3: {
		SetSpawnTypeBoss();
	}
	break;
	case 4: {
		SetSpawnTypeETC();
	}
	}
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