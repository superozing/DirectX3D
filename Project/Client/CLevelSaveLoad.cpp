#include "pch.h"
#include "CLevelSaveLoad.h"

#include <Engine/CPathMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CAssetMgr.h>

#include <Scripts/CScriptMgr.h>
#include <Engine/CScript.h>

#define TagLevelName "[LevelName]"
#define TagLayerName "[LayerName]"
#define TagObjCount "[ObjCount]"
#define TagObjectName "[ObjectName]"
#define TagComponentType "[ComponentType]"
#define TagComponentEnd "[ComponentEnd]"
#define TagScriptCount "[ScriptCount]"
#define TagScriptName "[ScriptName]"
#define TagScriptName "[ScriptName]"
#define TagObjectChildCountS "[" + ToString(_Obj->GetName()) + "'s ChildCount]"
#define TagObjectChildCountL "[" + ToString(pObject->GetName()) + "'s ChildCount]"

void CLevelSaveLoad::SaveLevel(CLevel* _Level, const wstring& _strLevelPath)
{
	assert(_Level);
	if (LEVEL_STATE::PLAY == _Level->GetState())
	{
		MessageBox(nullptr, L"레벨을 Stop 상태로 만들어야 합니다.", L"Level Save System", 0);
		return;
	}

	// Level 을 저장할 경로
	wstring strLevelPath = CPathMgr::GetContentPath();
	strLevelPath += _strLevelPath;

	ofstream fout(strLevelPath, ofstream::out | ofstream::trunc);
	if (!fout.is_open())
	{
		assert(nullptr);
	}

	// 레벨의 이름
	fout << TagLevelName << endl;
	fout << ToString(_Level->GetName()) << endl;

	// 레벨의 레이어 저장
	for (UINT i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		SaveLayer(_Level->GetLayer(i), fout);
	}
}

void CLevelSaveLoad::SaveCheckPoint(CLevel* _Level)
{
	assert(_Level);
	assert(LEVEL_STATE::PAUSE == _Level->GetState());

	wstring wpath	= L"level\\";
	wstring wLvName = L"CheckPoint.lv";

	_Level->SetName(wLvName);

	wpath += wLvName;
	SaveLevel(_Level, wpath);
}

void CLevelSaveLoad::SaveLayer(CLayer* _Layer, FILE* _File)
{
	// Layer 의 이름 저장
	SaveWString(_Layer->GetName(), _File);

	// Layer 가 보유하고 있는 GameObject 들을 저장
	const vector<CGameObject*>& vecObject = _Layer->GetParentObjects();

	size_t ObjCount = vecObject.size();
	fwrite(&ObjCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecObject.size(); ++i)
	{
		SaveGameObject(vecObject[i], _File);
	}
}

void CLevelSaveLoad::SaveLayer(CLayer* _Layer, ofstream& fout)
{
	// Layer 의 이름 저장
	fout << TagLayerName << endl;
	fout << ToString(_Layer->GetName()) << endl;

	// Layer 가 보유하고 있는 GameObject 들을 저장
	const vector<CGameObject*>& vecObject = _Layer->GetParentObjects();

	size_t ObjCount = vecObject.size();
	fout << TagObjCount << endl;
	fout << ObjCount << endl;

	for (size_t i = 0; i < vecObject.size(); ++i)
	{
		SaveGameObject(vecObject[i], fout);
	}
}

void CLevelSaveLoad::SaveGameObject(CGameObject* _Obj, FILE* _File)
{
	// GameObject 의 이름을 저장
	SaveWString(_Obj->GetName(), _File);

	// 컴포넌트 정보를 저장
	UINT i = 0;
	for (; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pCom = _Obj->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == pCom)
			continue;

		// 컴포넌트 타입 정보 저장
		fwrite(&i, sizeof(UINT), 1, _File);

		// 해당 컴포넌트가 저장할 데이터 저장
		pCom->SaveToFile(_File);
	}
	fwrite(&i, sizeof(UINT), 1, _File);

	// 스크립트 정보 저장
	const vector<CScript*>& vecScripts	= _Obj->GetScripts();
	size_t					ScriptCount = vecScripts.size();

	// 스크립트 개수 저장
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		SaveWString(CScriptMgr::GetScriptName(vecScripts[i]), _File);
		vecScripts[i]->SaveToFile(_File);
	}

	// 자식 오브젝트가 있으면 자식 오브젝트 정보 저장
	const vector<CGameObject*>& vecChild   = _Obj->GetChild();
	size_t						childcount = vecChild.size();
	fwrite(&childcount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < childcount; ++i)
	{
		SaveGameObject(vecChild[i], _File);
	}
}

void CLevelSaveLoad::SaveGameObject(CGameObject* _Obj, ofstream& fout)
{
	// GameObject 의 이름을 저장
	fout << TagObjectName << endl;
	fout << ToString(_Obj->GetName()) << endl;
	fout << TagLayerName << endl;
	fout << _Obj->GetLayerIdx() << endl;

	// 컴포넌트 정보를 저장
	UINT i = 0;
	for (; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pCom = _Obj->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == pCom)
			continue;

		// 컴포넌트 타입 정보 저장
		fout << TagComponentType << endl;
		auto type = magic_enum::enum_name((COMPONENT_TYPE)i);
		fout << ToString(type) << endl;

		// 해당 컴포넌트가 저장할 데이터 저장
		pCom->SaveToFile(fout);
	}
	fout << TagComponentEnd << endl;

	// 스크립트 정보 저장
	const vector<CScript*>& vecScripts	= _Obj->GetScripts();
	size_t					ScriptCount = vecScripts.size();

	// 스크립트 개수 저장
	fout << TagScriptCount << endl;
	fout << ScriptCount << endl;

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		fout << TagScriptName << endl;
		fout << ToString(wstring(CScriptMgr::GetScriptName(vecScripts[i]))) << endl;
		vecScripts[i]->SaveToFile(fout);
	}

	// 자식 오브젝트가 있으면 자식 오브젝트 정보 저장
	const vector<CGameObject*>& vecChild   = _Obj->GetChild();
	size_t						childcount = vecChild.size();

	string strChildCount;
	strChildCount = TagObjectChildCountS;
	fout << strChildCount << endl;
	fout << childcount << endl;

	for (size_t i = 0; i < childcount; ++i)
	{
		SaveGameObject(vecChild[i], fout);
	}
}

CLevel* CLevelSaveLoad::LoadLevel(const wstring& _strLevelPath)
{
	CLevel* pLevel = nullptr;

	// Level 을 불러올 경로
	wstring strLevelPath = CPathMgr::GetContentPath();
	strLevelPath += _strLevelPath;

	ifstream fin(strLevelPath);

	if (!fin.is_open())
	{
		MessageBox(nullptr, L"레벨 경로가 올바르지 않거나, 레벨이 없음", L"레벨 로드 실패", MB_OK);
		return nullptr;
	}

	// 레벨의 이름을 읽는다.
	pLevel = new CLevel;
	string strLevelName;

	Utils::GetLineUntilString(fin, TagLevelName);
	getline(fin, strLevelName);
	pLevel->SetName(strLevelName);
	pLevel->SetRelativePath(ToString(_strLevelPath));

	// Layer 로드
	for (UINT i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		LoadLayer(pLevel->GetLayer(i), fin);
	}

	return pLevel;
}

void CLevelSaveLoad::LoadLayer(CLayer* _Layer, FILE* _File)
{
	// Layer 의 이름 저장
	wstring strLayerName;
	LoadWString(strLayerName, _File);
	_Layer->SetName(strLayerName);

	// Layer 가 보유하던 GameObject 들을 불러온다.
	size_t ObjCount = 0;
	fread(&ObjCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ObjCount; ++i)
	{
		CGameObject* pObject = LoadGameObject(_File);
		_Layer->AddObject(pObject, false);
		_Layer->SetName(ToString(magic_enum::enum_name((LAYER)i)));
	}
}

void CLevelSaveLoad::LoadLayer(CLayer* _Layer, ifstream& fin)
{
	string tag;
	string str;
	Utils::GetLineUntilString(fin, TagLayerName);
	getline(fin, str);

	int objCnt;
	Utils::GetLineUntilString(fin, TagObjCount);
	fin >> objCnt;

	for (size_t i = 0; i < objCnt; i++)
	{
		CGameObject* pObject = LoadGameObject(fin);
		_Layer->AddObject(pObject, false, true);
	}
}

CGameObject* CLevelSaveLoad::LoadGameObject(FILE* _File)
{
	CGameObject* pObject = new CGameObject;

	// GameObject 의 이름을 로드
	wstring strName;
	LoadWString(strName, _File);
	pObject->SetName(strName);

	// 컴포넌트 정보를 불러오기
	COMPONENT_TYPE type = COMPONENT_TYPE::END;
	while (true)
	{
		fread(&type, sizeof(UINT), 1, _File);
		if (COMPONENT_TYPE::END == type)
			break;

		CComponent* pComponent = nullptr;

		switch (type)
		{
		case COMPONENT_TYPE::TRANSFORM:
			pComponent = new CTransform;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			pComponent = new CCollider2D;
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			pComponent = new CAnimator2D;
			break;
		case COMPONENT_TYPE::LIGHT2D:
			pComponent = new CLight2D;
			break;
		case COMPONENT_TYPE::LIGHT3D:
			pComponent = new CLight3D;
			break;
		case COMPONENT_TYPE::CAMERA:
			pComponent = new CCamera;
			break;
		case COMPONENT_TYPE::STATEMACHINE:
			pComponent = new CStateMachine;
			break;
		case COMPONENT_TYPE::PHYSX:
			pComponent = new CPhysX;
			break;
		case COMPONENT_TYPE::MESHRENDER:
			pComponent = new CMeshRender;
			break;
		case COMPONENT_TYPE::TILEMAP:
			pComponent = new CTileMap;
			break;
		case COMPONENT_TYPE::PARTICLESYSTEM:
			pComponent = new CParticleSystem;
			break;
		case COMPONENT_TYPE::SKYBOX:
			pComponent = new CSkyBox;
			break;
		case COMPONENT_TYPE::DECAL:
			pComponent = new CDecal;
			break;
		case COMPONENT_TYPE::LANDSCAPE:
			pComponent = new CLandScape;
		default:
			assert(nullptr);
			break;
		}

		// 해당 컴포넌트가 저장한 데이터를 로드
		pObject->AddComponent(pComponent);
		pComponent->LoadFromFile(_File);
	}

	// 스크립트 개수 읽기
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring strScriptName;
		LoadWString(strScriptName, _File);

		CScript* pScript = CScriptMgr::GetScript(strScriptName);
		pObject->AddComponent(pScript);
		pScript->LoadFromFile(_File);
	}

	// 자식 오브젝트가 있으면 자식 오브젝트 정보 저장
	size_t childcount = 0;
	fread(&childcount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < childcount; ++i)
	{
		pObject->AddChild(LoadGameObject(_File));
	}

	return pObject;
}

CGameObject* CLevelSaveLoad::LoadGameObject(ifstream& fin)
{
	CGameObject* pObject = new CGameObject;

	string tag, str;
	Utils::GetLineUntilString(fin, TagObjectName);
	getline(fin, str);
	pObject->SetName(str);

	// NOTE : 레벨 로드할 때 실패하면 아래 2줄 주석처리 해주어야 함
	// Utils::GetLineUntilString(fin, TagLayerName);
	// fin >> pObject->m_iLayerIdx;

	while (true)
	{
		tag = Utils::GetLineUntilString(fin, {TagComponentType, TagComponentEnd});
		if (tag == TagComponentEnd)
			break;

		string strComponent;
		strComponent = str;
		getline(fin, str);
		auto num = magic_enum::enum_cast<COMPONENT_TYPE>(str);
		if (num.has_value())
		{
			auto		type	   = num.value();
			CComponent* pComponent = nullptr;

			switch (type)
			{
			case COMPONENT_TYPE::TRANSFORM:
				pComponent = new CTransform;
				break;
			case COMPONENT_TYPE::COLLIDER2D:
				pComponent = new CCollider2D;
				break;
			case COMPONENT_TYPE::COLLIDER3D:
				MessageBox(nullptr, L"Collider3D", L"세이브/로드가 구현되지 않은 컴포넌트", 0);
				break;
			case COMPONENT_TYPE::ANIMATOR2D:
				pComponent = new CAnimator2D;
				break;
			case COMPONENT_TYPE::ANIMATOR3D:
				pComponent = new CAnimator3D;
				break;
			case COMPONENT_TYPE::LIGHT2D:
				pComponent = new CLight2D;
				break;
			case COMPONENT_TYPE::LIGHT3D:
				pComponent = new CLight3D;
				break;
			case COMPONENT_TYPE::CAMERA:
				pComponent = new CCamera;
				break;
			case COMPONENT_TYPE::STATEMACHINE:
				pComponent = new CStateMachine;
				break;
			case COMPONENT_TYPE::PHYSX:
				pComponent = new CPhysX;
				break;
			case COMPONENT_TYPE::MESHRENDER:
				pComponent = new CMeshRender;
				break;
			case COMPONENT_TYPE::TILEMAP:
				pComponent = new CTileMap;
				break;
			case COMPONENT_TYPE::PARTICLESYSTEM:
				pComponent = new CParticleSystem;
				break;
			case COMPONENT_TYPE::SKYBOX:
				pComponent = new CSkyBox;
				break;
			case COMPONENT_TYPE::DECAL:
				pComponent = new CDecal;
				break;
			case COMPONENT_TYPE::LANDSCAPE:
				pComponent = new CLandScape;
				break;
			default:
				assert(nullptr);
				break;
			}

			// 해당 컴포넌트가 저장한 데이터를 로드
			pObject->AddComponent(pComponent);
			pComponent->LoadFromFile(fin);
		}
	}

	size_t scriptCnt;
	Utils::GetLineUntilString(fin, TagScriptCount);
	fin >> scriptCnt;

	for (size_t i = 0; i < scriptCnt; i++)
	{
		Utils::GetLineUntilString(fin, TagScriptName);
		getline(fin, str);
		CScript* pScript = CScriptMgr::GetScript(ToWString(str));
		pScript->LoadFromFile(fin);
		pObject->AddComponent(pScript);
	}

	size_t childCnt;
	Utils::GetLineUntilString(fin, TagObjectChildCountL);
	fin >> childCnt;

	for (size_t i = 0; i < childCnt; i++)
	{
		pObject->AddChild(LoadGameObject(fin), true);
	}

	return pObject;
}
