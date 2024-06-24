#include "pch.h"
#include "CMaterial.h"

#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTexture.h"

#include "CPathMgr.h"

#include "Ptr.h"

CMaterial::CMaterial(bool _bEngine)
	: CAsset(ASSET_TYPE::MATERIAL, _bEngine)
	, m_Const{}
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::UpdateData()
{
	if (nullptr == m_pShader.Get())
		return;

	m_pShader->UpdateData();

	// Texture Update(Register Binding)
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		if (nullptr != m_arrTex[i].Get())
		{
			m_arrTex[i]->UpdateData(i);
			m_Const.bTex[i] = 1;
		}
		else
		{
			CTexture::Clear(i);
			m_Const.bTex[i] = 0;
		}
	}

	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
	pCB->SetData(&m_Const);
	pCB->UpdateData();
}

void CMaterial::UpdateData_Inst()
{
	if (nullptr == m_pShader)
		return;

	// 사용할 쉐이더 바인딩
	m_pShader->UpdateData_Inst();

	// Texture Update(Register Binding)
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		if (nullptr != m_arrTex[i].Get())
		{
			m_arrTex[i]->UpdateData(i);
			m_Const.bTex[i] = 1;
		}
		else
		{
			CTexture::Clear(i);
			m_Const.bTex[i] = 0;
		}
	}

	// 상수 데이터 업데이트
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
	pCB->SetData(&m_Const);
	pCB->UpdateData();
}

void CMaterial::SetTexParam(TEX_PARAM _Param, Ptr<CTexture> _pTex)
{
	m_arrTex[(UINT)_Param] = _pTex;
}

void* CMaterial::GetScalarParam(SCALAR_PARAM _ParamType)
{
	switch (_ParamType)
	{
	case SCALAR_PARAM::BOOL_0:
	case SCALAR_PARAM::BOOL_1:
	case SCALAR_PARAM::BOOL_2:
	case SCALAR_PARAM::BOOL_3: {
		int idx = (UINT)_ParamType - (UINT)SCALAR_PARAM::BOOL_0;
		return m_Const.bArr + idx;
	}
	break;
	case SCALAR_PARAM::INT_0:
	case SCALAR_PARAM::INT_1:
	case SCALAR_PARAM::INT_2:
	case SCALAR_PARAM::INT_3: {
		int idx = (UINT)_ParamType - (UINT)SCALAR_PARAM::INT_0;
		return m_Const.iArr + idx;
	}
	break;
	case SCALAR_PARAM::FLOAT_0:
	case SCALAR_PARAM::FLOAT_1:
	case SCALAR_PARAM::FLOAT_2:
	case SCALAR_PARAM::FLOAT_3: {
		int idx = (UINT)_ParamType - (UINT)SCALAR_PARAM::FLOAT_0;
		return m_Const.fArr + idx;
	}
	break;
	case SCALAR_PARAM::VEC2_0:
	case SCALAR_PARAM::VEC2_1:
	case SCALAR_PARAM::VEC2_2:
	case SCALAR_PARAM::VEC2_3: {
		int idx = (UINT)_ParamType - (UINT)SCALAR_PARAM::VEC2_0;
		return m_Const.v2Arr + idx;
	}
	break;
	case SCALAR_PARAM::VEC4_0:
	case SCALAR_PARAM::VEC4_1:
	case SCALAR_PARAM::VEC4_2:
	case SCALAR_PARAM::VEC4_3: {
		int idx = (UINT)_ParamType - (UINT)SCALAR_PARAM::VEC4_0;
		return m_Const.v4Arr + idx;
	}
	break;
	case SCALAR_PARAM::MAT_0:
	case SCALAR_PARAM::MAT_1:
	case SCALAR_PARAM::MAT_2:
	case SCALAR_PARAM::MAT_3: {
		int idx = (UINT)_ParamType - (UINT)SCALAR_PARAM::MAT_0;
		return m_Const.matArr + idx;
	}
	break;
	}

	return nullptr;
}

#define TagMtrlConst "[MtrlConst]"
#define TagTexture "[Textures]"
#define TagMtrlShader "[MtrlShader]"
int CMaterial::Save(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetContentPath();
	strFilePath += _strRelativePath;

	ofstream fout(strFilePath);
	if (!fout.is_open())
		return E_FAIL;

	fout << TagMtrlConst << endl;
	fout << m_Const << endl;

	// 재질이 참조하는 텍스쳐 정보를 저장
	fout << TagTexture << endl;
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		SaveAssetRef<CTexture>(m_arrTex[i], fout);
	}

	// 재질이 참조하는 쉐이더 정보를 저장
	fout << TagMtrlShader << endl;
	SaveAssetRef<CGraphicsShader>(m_pShader, fout);

	return 0;
}

int CMaterial::Load(const wstring& _strFilePath)
{
	ifstream fin(_strFilePath);
	if (!fin.is_open())
		return E_FAIL;

	Utils::GetLineUntilString(fin, TagMtrlConst);
	fin >> m_Const;

	// 재질이 참조하는 텍스쳐 정보를 로드
	Utils::GetLineUntilString(fin, TagTexture);
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		LoadAssetRef<CTexture>(m_arrTex[i], fin);
	}

	// 재질이 참조하는 쉐이더 정보를 저장
	Utils::GetLineUntilString(fin, TagMtrlShader);
	LoadAssetRef<CGraphicsShader>(m_pShader, fin);

	return 0;
}