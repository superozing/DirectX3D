#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

SAVE_TYPE CPrefab::GAMEOBJECT_SAVE = nullptr;
LOAD_TYPE CPrefab::GAMEOBJECT_LOAD = nullptr;

CPrefab::CPrefab(bool _Engine)
	: CAsset(ASSET_TYPE::PREFAB, _Engine)
	, m_ProtoObj(nullptr)
{
}

CPrefab::CPrefab(CGameObject* _Proto, bool _Engine)
	: CAsset(ASSET_TYPE::PREFAB, _Engine)
	, m_ProtoObj(_Proto)
{
}

CPrefab::~CPrefab()
{
	if (nullptr != m_ProtoObj)
		delete m_ProtoObj;
}

void CPrefab::SetGameObject(CGameObject* _Object)
{
	if (nullptr != m_ProtoObj)
		delete m_ProtoObj;

	m_ProtoObj = _Object;
}

CGameObject* CPrefab::Instantiate()
{
	return m_ProtoObj->Clone();
}

int CPrefab::Save(const wstring& _strRelativePath)
{
	assert(GAMEOBJECT_SAVE);

	wstring strContentPath = CPathMgr::GetContentPath();
	strContentPath += _strRelativePath;

	ofstream fout(strContentPath);
	if (!fout.is_open())
		return E_FAIL;

	GAMEOBJECT_SAVE(m_ProtoObj, fout);

	return S_OK;
}

int CPrefab::Load(const wstring& _strFilePath)
{
	assert(GAMEOBJECT_LOAD);

	ifstream fin(_strFilePath);
	if (!fin.is_open())
		return E_FAIL;

	m_ProtoObj = GAMEOBJECT_LOAD(fin);

	return S_OK;
}