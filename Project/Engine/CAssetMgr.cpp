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
	if (_relativePath.extension() == L".gs")
		return ASSET_TYPE::GRAPHICS_SHADER;

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

void CAssetMgr::SaveDefaultGraphicsShader()
{
	Ptr<CGraphicsShader> pShader = nullptr;
	wstring path = L"GraphicsShader\\";
	wstring ext = L"_src.gs";

	// =================================
	// Std2DShader
	// =================================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXstd2d, FN_VS_std2d);
	pShader->CreatePixelShader(FXstd2d, FN_PS_std2d);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	// Parameter	
	pShader->AddScalarParam(SCALAR_PARAM::INT_0, "Test Param");
	pShader->AddTexParam(TEX_PARAM::TEX_0, "Output Texture 1");

	pShader->Save(path + SHADER_std2d + ext);

	// ===========
	// Std3DShader
	// ===========
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXstd3d, FN_VS_std3d);
	pShader->CreatePixelShader(FXstd3d, FN_PS_std3d);

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	//pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	pShader->AddScalarParam(SCALAR_PARAM::INT_0, "Lighting Type", 0.f, 0.f, false, "0 : Gouraud, 1 : Phong");
	pShader->AddTexParam(TEX_PARAM::TEX_0, "Output Texture");
	pShader->AddTexParam(TEX_PARAM::TEX_1, "Normal Texture");

	pShader->Save(path + SHADER_std3d + ext);

	// ====================
	// Std3D_DeferredShader
	// ====================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXstd3d_deferred, FN_VS_deferred);
	pShader->CreatePixelShader(FXstd3d_deferred, FN_PS_deferred);

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

	pShader->Save(path + L"Std3D_DeferredShader" + ext);

	// ==============
	// DirLightShader
	// ==============
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXlight, FN_VS_dirlight);
	pShader->CreatePixelShader(FXlight, FN_PS_dirlight);

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHTING);

	pShader->Save(path + SHADER_dirlight + ext);

	// =================
	// PointLight Shader
	// =================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXlight, FN_VS_pointlight);
	pShader->CreatePixelShader(FXlight, FN_PS_pointlight);

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHTING);

	pShader->Save(path + SHADER_pointlight + ext);

	// =================
	// SpotLight Shader
	// =================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXlight, FN_VS_spotlight);
	pShader->CreatePixelShader(FXlight, FN_PS_spotlight);

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHTING);

	pShader->Save(path + SHADER_spotlight + ext);

	// ============
	// Merge Shader
	// ============
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXmerge, FN_VS_merge);
	pShader->CreatePixelShader(FXmerge, FN_PS_merge);

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MERGE);

	pShader->Save(path + SHADER_merge + ext);

	// ============
	// Decal Shader
	// ============
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\decal.fx", "VS_Decal");
	pShader->CreatePixelShader(L"shader\\decal.fx", "PS_Decal");

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DECAL);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DECAL);

	// Parameter
	pShader->AddTexParam(TEX_PARAM::TEX_0, "Decal Texture");

	pShader->Save(path + L"DecalShader" + ext);

	// =================================
	// EffectShader
	// =================================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXstd2d, FN_VS_std2d);
	pShader->CreatePixelShader(FXstd2d, FN_PS_effect);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	pShader->Save(path + SHADER_effect + ext);


	// =============
	// TileMapShader
	// =============
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXtilemap, FN_VS_tile);
	pShader->CreatePixelShader(FXtilemap, FN_PS_tile);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	pShader->Save(path + SHADER_tilemap + ext);

	// ====================
	// ParticleRenderShader
	// ====================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXparticle, FN_VS_particle);
	pShader->CreateGeometryShader(FXparticle, FN_GS_particle);
	pShader->CreatePixelShader(FXparticle, FN_PS_particle);

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);		// 깊이 테스트는 진행, 깊이는 기록 X
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	pShader->Save(path + SHADER_particlerender + ext);

	// =================================
	// GrayFilter Shader
	// Mesh			: RectMesh
	// RS_TYPE		: CULL_BACK
	// DS_TYPE		: NO_TEST_NO_WRITE
	// BS_TYPE		: Default
	// Domain		: DOMAIN_POSTPROCESS
	// =================================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXpostprocess, FN_VS_grayfilter);
	pShader->CreatePixelShader(FXpostprocess, FN_PS_grayfilter);

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	pShader->Save(path + SHADER_grayfilter + ext);

	// ============
	// SkyBoxShader
	// ============
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXskybox, FN_VS_skybox);
	pShader->CreatePixelShader(FXskybox, FN_PS_skybox);

	pShader->SetRSType(RS_TYPE::CULL_FRONT);

	// SkyBox 는 최대깊이인 1 로 그려질 예정, 따라서 초기화 값인 Depth 1 이랑 같은 경우까지도 통과시켜줘야 한다.
	pShader->SetDSType(DS_TYPE::LESS_EQUAL);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

	pShader->Save(path + SHADER_skybox + ext);

	// =================================
	// Distortion Shader	
	// RS_TYPE		: CULL_BACK
	// DS_TYPE		: NO_TEST_NO_WRITE
	// BS_TYPE		: Default
	// Domain		: DOMAIN_POSTPROCESS
	// =================================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXpostprocess, FN_VS_distortion);
	pShader->CreatePixelShader(FXpostprocess, FN_PS_distortion);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	pShader->Save(path + SHADER_distortion + ext);

	// =================================
	// DebugShape Shader
	// =================================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(FXdebug, FN_VS_debugshape);
	pShader->CreatePixelShader(FXdebug, FN_PS_debugshape);

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);
	pShader->Save(path + SHADER_debugshape + ext);

	// ================
	// ShadowMap Shader
	// ================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\shadowmap.fx", "VS_ShadowMap");
	pShader->CreatePixelShader(L"shader\\shadowmap.fx", "PS_ShadowMap");

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_SHADOWMAP);

	pShader->Save(path + L"ShadowMapShader" + ext);
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
	pMeshData->Save(strName);

	return pMeshData;
}
