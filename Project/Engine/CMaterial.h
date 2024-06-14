#pragma once
#include "CAsset.h"

#include "CTexture.h"
#include "CGraphicsShader.h"

class CMaterial :
    public CAsset
{
private:
    tMtrlConst				m_Const;
	Ptr<CTexture>			m_arrTex[(UINT)TEX_PARAM::END];
    Ptr<CGraphicsShader>    m_pShader;
    
public:
    void SetShader(Ptr<CGraphicsShader> _Shader) { m_pShader = _Shader; }
	Ptr<CGraphicsShader> GetShader() { return m_pShader; }

    template<typename T>
    void SetScalarParam(SCALAR_PARAM _ParamType, const T& _Value);
	void SetTexParam(TEX_PARAM _Param, Ptr<CTexture> _pTex);

	void SetMaterialCoefficient(Vec4 _vDiff, Vec4 _vSpec, Vec4 _vAmb, Vec4 _vEmis)
	{
		m_Const.mtrl.vDiff = _vDiff;
		m_Const.mtrl.vAmb = _vAmb;
		m_Const.mtrl.vSpec = _vSpec;
		m_Const.mtrl.vEmv = _vEmis;
	}

	void SetAnim3D(bool _bTrue) { m_Const.arrAnimData[0] = (int)_bTrue; }
	void SetBoneCount(int _iBoneCount) { m_Const.arrAnimData[1] = _iBoneCount; }

	void* GetScalarParam(SCALAR_PARAM _ParamType);
	Ptr<CTexture> GetTexParam(TEX_PARAM _ParamType) { return m_arrTex[(UINT)_ParamType]; }

    void UpdateData();

	void operator = (const CMaterial& _OtherMtrl)
	{
		SetName(_OtherMtrl.GetName());

		m_Const = _OtherMtrl.m_Const;

		for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
		{
			m_arrTex[i] = _OtherMtrl.m_arrTex[i];
		}

		m_pShader = _OtherMtrl.m_pShader;
	}

	virtual int Save(const wstring& _strRelativePath);
	virtual int Load(const wstring& _strFilePath);
	virtual int Save(const string& _strRelativePath) { return Save(ToWString(_strRelativePath)); }
	virtual int Load(const string& _strFilePath) { return Load(ToWString(_strFilePath)); }

public:
	CLONE(CMaterial);
	CMaterial(bool _bEngine = false);
    ~CMaterial();
};


template<typename T>
void CMaterial::SetScalarParam(SCALAR_PARAM _ParamType, const T& _Value)
{
	const T* pValue = &_Value;

	switch (_ParamType)
	{
	case SCALAR_PARAM::BOOL_0:
	case SCALAR_PARAM::BOOL_1:
	case SCALAR_PARAM::BOOL_2:
	case SCALAR_PARAM::BOOL_3:
		m_Const.bArr[(UINT)_ParamType] = *((bool*)pValue);
		break;

	case SCALAR_PARAM::INT_0:
	case SCALAR_PARAM::INT_1:
	case SCALAR_PARAM::INT_2:
	case SCALAR_PARAM::INT_3:
		m_Const.iArr[(UINT)_ParamType - (UINT)SCALAR_PARAM::INT_0] = *((int*)pValue);
		break;

	case SCALAR_PARAM::FLOAT_0:
	case SCALAR_PARAM::FLOAT_1:
	case SCALAR_PARAM::FLOAT_2:
	case SCALAR_PARAM::FLOAT_3:
		m_Const.fArr[(UINT)_ParamType - (UINT)SCALAR_PARAM::FLOAT_0] = *((float*)pValue);
		break;

	case SCALAR_PARAM::VEC2_0:
	case SCALAR_PARAM::VEC2_1:
	case SCALAR_PARAM::VEC2_2:
	case SCALAR_PARAM::VEC2_3:
		m_Const.v2Arr[(UINT)_ParamType - (UINT)SCALAR_PARAM::VEC2_0] = *((Vec2*)pValue);


		break;
	case SCALAR_PARAM::VEC4_0:
	case SCALAR_PARAM::VEC4_1:
	case SCALAR_PARAM::VEC4_2:
	case SCALAR_PARAM::VEC4_3:
		m_Const.v4Arr[(UINT)_ParamType - (UINT)SCALAR_PARAM::VEC4_0] = *((Vec4*)pValue);

		break;
	case SCALAR_PARAM::MAT_0:
	case SCALAR_PARAM::MAT_1:
	case SCALAR_PARAM::MAT_2:
	case SCALAR_PARAM::MAT_3:
		m_Const.matArr[(UINT)_ParamType - (UINT)SCALAR_PARAM::MAT_0] = *((Matrix*)pValue);
		break;
	}
}