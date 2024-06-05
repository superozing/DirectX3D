#include "pch.h"
#include "CAssetMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"

#include "CSound.h"

CAssetMgr::CAssetMgr()
{
	
}

CAssetMgr::~CAssetMgr()
{
	
}

void CAssetMgr::exit()
{
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		for (auto it = m_mapAsset[i].begin(); it != m_mapAsset[i].end(); )
		{
			// 엔진 에셋이 아닌 에셋을 맵에서 제거
			if (!it->second->IsEngineAsset())
				it = m_mapAsset[i].erase(it);
			else
				++it;
		}
	}

}

void CAssetMgr::AddAsset(const wstring& _strKey, CAsset* _Asset)
{
	ASSET_TYPE Type = _Asset->GetType();

	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);

	if (iter != m_mapAsset[(UINT)Type].end())
	{
		iter->second = _Asset;
		return;
	}
	assert(iter == m_mapAsset[(UINT)Type].end());

	_Asset->SetKey(_strKey);
	_Asset->SetRelativePath(_strKey);
	m_mapAsset[(UINT)Type].insert(make_pair(_strKey, _Asset));
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _strKey
									 , UINT _Width, UINT _Height, DXGI_FORMAT _Format
									 , UINT _Flag, D3D11_USAGE _Usage)
{
	// 생성하려는 텍스쳐와 동일한 키의 텍스쳐가 이미 AssetMgr 에 있다면
	Ptr<CTexture> pTex = FindAsset<CTexture>(_strKey);	
	assert(!pTex.Get());

	pTex = new CTexture(true);

	if (FAILED(pTex->Create(_Width, _Height, _Format, _Flag, _Usage)))
	{
		MessageBox(nullptr, L"텍스쳐 생성 실패", L"리소스 생성 실패", MB_OK);
		return nullptr;
	}

	AddAsset<CTexture>(_strKey, pTex.Get());

	return pTex;
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _tex2D)
{
	// 생성하려는 텍스쳐와 동일한 키의 텍스쳐가 이미 AssetMgr 에 있다면
	Ptr<CTexture> pTex = FindAsset<CTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new CTexture(true);

	if (FAILED(pTex->Create(_tex2D)))
	{
		MessageBox(nullptr, L"텍스쳐 생성 실패", L"리소스 생성 실패", MB_OK);
		return nullptr;
	}

	AddAsset<CTexture>(_strKey, pTex.Get());

	return pTex;
}

void CAssetMgr::DeleteAsset(ASSET_TYPE _Type, const wstring& _strKey)
{
	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)_Type].find(_strKey);

	assert(!(iter == m_mapAsset[(UINT)_Type].end()));

	m_mapAsset[(UINT)_Type].erase(iter);
}

void CAssetMgr::GetAssetName(ASSET_TYPE _Type, vector<string>& _Out)
{
	for (const auto& pair : m_mapAsset[(UINT)_Type])
	{
		_Out.push_back(ToString(pair.first));
	}
}

ASSET_TYPE CAssetMgr::GetAssetTypeByExt(const path& _relativePath)
{
	if (_relativePath.extension() == L".mesh")
		return ASSET_TYPE::MESH;
	if (_relativePath.extension() == L".mtrl")
		return ASSET_TYPE::MATERIAL;
	if (_relativePath.extension() == L".mdat")
		return ASSET_TYPE::MESHDATA;
	if (_relativePath.extension() == L".pref")
		return ASSET_TYPE::PREFAB;

	if (_relativePath.extension() == L".png"
		|| _relativePath.extension() == L".bmp"
		|| _relativePath.extension() == L".jpg"
		|| _relativePath.extension() == L".jpeg"
		|| _relativePath.extension() == L".dds"
		|| _relativePath.extension() == L".tga")
		return ASSET_TYPE::TEXTURE;

	if (_relativePath.extension() == L".wav"
		|| _relativePath.extension() == L".mp3"
		|| _relativePath.extension() == L".ogg")
		return ASSET_TYPE::SOUND;

	if (_relativePath.extension() == L".mesh")
		return ASSET_TYPE::MESH;
	if (_relativePath.extension() == L".mesh")
		return ASSET_TYPE::MESH;

	return ASSET_TYPE::END;
}

Ptr<CAsset> CAssetMgr::GetAsset(ASSET_TYPE _type, string _key)
{
	Ptr<CAsset> pAsset;

	switch (_type)
	{
	case ASSET_TYPE::MESH:
		pAsset = (CAsset*)CAssetMgr::GetInst()->Load<CMesh>(_key).Get();
		break;
	case ASSET_TYPE::MESHDATA:
		//pAsset = (CAsset*)CAssetMgr::GetInst()->Load<CMeshData>(key).Get();
		break;
	case ASSET_TYPE::PREFAB:
		pAsset = (CAsset*)CAssetMgr::GetInst()->Load<CPrefab>(_key).Get();
		break;
	case ASSET_TYPE::TEXTURE:
		pAsset = (CAsset*)CAssetMgr::GetInst()->Load<CTexture>(_key).Get();
		break;
	case ASSET_TYPE::MATERIAL:
		pAsset = (CAsset*)CAssetMgr::GetInst()->Load<CMaterial>(_key).Get();
		break;
	case ASSET_TYPE::SOUND:
		pAsset = (CAsset*)CAssetMgr::GetInst()->Load<CSound>(_key).Get();
		break;
		// 이건 제외해야 함
	//case ASSET_TYPE::COMPUTE_SHADER:
	//	pAsset = (CAsset*)CAssetMgr::GetInst()->Load<CComputeShader>(key).Get();
	//	break;
	case ASSET_TYPE::GRAPHICS_SHADER:
		pAsset = (CAsset*)CAssetMgr::GetInst()->Load<CGraphicsShader>(_key).Get();
		break;
	case ASSET_TYPE::FSM:
		pAsset = (CAsset*)CAssetMgr::GetInst()->Load<CFSM>(_key).Get();
		break;
	case ASSET_TYPE::END:
		break;
	default:
		break;
	}
	return pAsset;
}

Ptr<CMeshData> CAssetMgr::LoadFBX(const wstring& _strPath)
{
	wstring strFileName = path(_strPath).stem();

	wstring strName = L"meshdata\\";
	strName += strFileName + L".mdat";

	Ptr<CMeshData> pMeshData = FindAsset<CMeshData>(strName);

	if (nullptr != pMeshData)
		return pMeshData;

	pMeshData = CMeshData::LoadFromFBX(_strPath);
	pMeshData->SetKey(strName);
	pMeshData->SetRelativePath(strName);

	m_mapAsset[(UINT)ASSET_TYPE::MESHDATA].insert(make_pair(strName, pMeshData.Get()));

	// meshdata 를 실제파일로 저장
	//pMeshData->Save(strName);

	return pMeshData;
}
