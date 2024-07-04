﻿#pragma once
#include "CManager.h"

#include "CPathMgr.h"

#include "CTexture.h"
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CComputeShader.h"
#include "CMaterial.h"
#include "CPrefab.h"
#include "CSound.h"
#include "CFSM.h"
#include "CMeshData.h"

template <typename T1, typename T2> constexpr bool MyBool		  = false;
template <typename T1> constexpr bool			   MyBool<T1, T1> = true;

class CAssetMgr : public CManager<CAssetMgr>
{
	SINGLE(CAssetMgr);

private:
	map<wstring, Ptr<CAsset>> m_mapAsset[(UINT)ASSET_TYPE::END];

public:
	virtual void init() override;
	virtual void enter() override {}
	virtual void exit() override;

private:
	void InitSound();
	void CreateDefaultMesh();
	void CreateDefaultGraphicsShader();
	void CreateDefaultComputeShader();
	void CreateDefaultMaterial();

public:
	template <typename T> const map<wstring, Ptr<CAsset>>& GetAssets();

	const map<wstring, Ptr<CAsset>>& GetAssets(ASSET_TYPE _Type) { return m_mapAsset[(UINT)_Type]; }

	template <typename T> void AddAsset(const wstring& _strKey, T* _Asset);

	void AddAsset(const wstring& _strKey, CAsset* _Asset);

	Ptr<CMeshData> LoadFBX(const wstring& _strPath);

	template <typename T> Ptr<T> FindAsset(const wstring& _strKey);

	template <typename T> Ptr<T> Load(const wstring& _strKey, const wstring& _strRelativePath);
	template <typename T> Ptr<T> Load(const wstring& _strSameKeyPath);
	template <typename T> Ptr<T> Load(string& _strKey, string& _strRelativePath);
	template <typename T> Ptr<T> Load(string& _strSameKeyPath);

	// _Flag : D3D11_BIND_FLAG
	Ptr<CTexture> CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _Flag,
								D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
	Ptr<CTexture> CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _tex2D);

	// 지정된 타입의 모든 에셋의 이름을 받아온다
	void GetAssetName(ASSET_TYPE _Type, vector<string>& _Out);

	/// <summary>
	/// 확장자로 애셋 타입을 가져옵니다. 등록되지 않은 확장자는 ASSET_TYPE::END를 리턴합니다
	/// </summary>
	ASSET_TYPE GetAssetTypeByExt(const path& _relativePath);

	/// <summary>
	/// 애셋 타입과 키로 애셋을 가져옵니다.
	/// </summary>
	Ptr<CAsset> GetAsset(ASSET_TYPE _type, string _key);

	void SaveDefaultGraphicsShader();

private:
	//  지정된 에셋을 삭제한다.
	template <typename T> void DeleteAsset(const wstring& _strKey);
	void					   DeleteAsset(ASSET_TYPE _Type, const wstring& _strKey);

	friend class CTaskMgr;
	friend class CDevice;
};

template <typename T> ASSET_TYPE GetAssetType()
{
	ASSET_TYPE Type = ASSET_TYPE::END;

	if constexpr (std::is_same_v<CMeshData, T>)
		Type = ASSET_TYPE::MESHDATA;
	if constexpr (std::is_same_v<CMesh, T>)
		Type = ASSET_TYPE::MESH;
	if constexpr (std::is_same_v<CTexture, T>)
		Type = ASSET_TYPE::TEXTURE;
	if constexpr (MyBool<CGraphicsShader, T>)
		Type = ASSET_TYPE::GRAPHICS_SHADER;
	if constexpr (std::is_same_v<CComputeShader, T>)
		Type = ASSET_TYPE::COMPUTE_SHADER;
	if constexpr (std::is_same_v<CMaterial, T>)
		Type = ASSET_TYPE::MATERIAL;
	if constexpr (std::is_same_v<CPrefab, T>)
		Type = ASSET_TYPE::PREFAB;
	if constexpr (std::is_same_v<CSound, T>)
		Type = ASSET_TYPE::SOUND;
	if constexpr (std::is_same_v<CFSM, T>)
		Type = ASSET_TYPE::FSM;

	return Type;
}

template <typename T> inline const map<wstring, Ptr<CAsset>>& CAssetMgr::GetAssets()
{
	ASSET_TYPE Type = GetAssetType<T>();

	return m_mapAsset[(UINT)Type];
}

template <typename T> inline void CAssetMgr::AddAsset(const wstring& _strKey, T* _Asset)
{
	ASSET_TYPE Type = GetAssetType<T>();

	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);
	if (iter != m_mapAsset[(UINT)Type].end())
	{
		iter->second = _Asset;
		return;
	}
	assert(iter == m_mapAsset[(UINT)Type].end());

	_Asset->SetKey(_strKey);
	m_mapAsset[(UINT)Type].insert(make_pair(_strKey, _Asset));
}

template <typename T> Ptr<T> CAssetMgr::FindAsset(const wstring& _strKey)
{
	ASSET_TYPE Type = GetAssetType<T>();

	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);

	if (iter == m_mapAsset[(UINT)Type].end())
	{
		return nullptr;
	}

	return (T*)iter->second.Get();
}

template <typename T> Ptr<T> CAssetMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
	Ptr<CAsset> pAsset = FindAsset<T>(_strKey).Get();

	// 로딩할 때 사용할 키로 이미 다른 에셋이 있다면
	if (nullptr != pAsset)
	{
		return (T*)pAsset.Get();
	}

	wstring strFilePath = CPathMgr::GetContentPath();
	strFilePath += _strRelativePath;

	pAsset = new T;
	if (FAILED(pAsset->Load(strFilePath)))
	{
		wstring str = _strKey;
		str += L" 에셋 로딩 실패";
		MessageBox(nullptr, str.c_str(), L"에셋 로딩 실패", MB_OK);
		pAsset = nullptr;
		return nullptr;
	}

	pAsset->SetKey(_strKey);
	pAsset->SetRelativePath(_strRelativePath);

	AddAsset<T>(_strKey, (T*)pAsset.Get());

	return (T*)pAsset.Get();
}

template <typename T> inline Ptr<T> CAssetMgr::Load(const wstring& _strSameKeyPath)
{
	return Load<T>(_strSameKeyPath, _strSameKeyPath);
}

template <typename T> Ptr<T> CAssetMgr::Load(string& _strKey, string& _strRelativePath)
{
	return Load<T>(ToWString(_strKey), ToWString(_strRelativePath));
}

template <typename T> inline Ptr<T> CAssetMgr::Load(string& _strSameKeyPath)
{
	return Load<T>(ToWString(_strSameKeyPath));
}

template <typename T> inline void CAssetMgr::DeleteAsset(const wstring& _strKey)
{
	ASSET_TYPE AssetType = GetAssetType<T>();

	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)AssetType].find(_strKey);

	assert(!(iter == m_mapAsset[(UINT)AssetType].end()));

	m_mapAsset[(UINT)AssetType].erase(iter);
}
