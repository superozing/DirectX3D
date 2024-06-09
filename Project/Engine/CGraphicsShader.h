#pragma once
#include "CShader.h"


struct tScalarParam
{
    SCALAR_PARAM    Type;
    string          Desc;
    float           Min;
    float           Max;
    bool            View;
    string          Tooltip;

    bool            IsUse;
};

struct tTexParam
{
    TEX_PARAM       Type;
    string          Desc;

    bool            IsUse;
};


class CGraphicsShader :
    public CShader
{
private:
    ComPtr<ID3DBlob>    m_VSBlob;
    ComPtr<ID3DBlob>    m_HSBlob;
    ComPtr<ID3DBlob>    m_DSBlob;
    ComPtr<ID3DBlob>    m_GSBlob;
    ComPtr<ID3DBlob>    m_PSBlob;

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11HullShader>        m_HS;
    ComPtr<ID3D11DomainShader>      m_DS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    ComPtr<ID3D11InputLayout>       m_Layout;
    D3D11_PRIMITIVE_TOPOLOGY        m_Topology;

    // Rasterizer State
    RS_TYPE                         m_RSType;

    // OutputMerge
    // DepthStencilState
    DS_TYPE                         m_DSType;
     
    // BlendState
    BS_TYPE                         m_BSType;

    // Shader Domain
    SHADER_DOMAIN                   m_Domain;

    // Shader 파라미터 목록
    tScalarParam            m_ScalarParam[(UINT)SCALAR_PARAM::END];
    tTexParam               m_TexParam[(UINT)TEX_PARAM::END];

    // Shader String Info
    string                          m_VSPath;
    string                          m_HSPath;
    string                          m_DSPath;
    string                          m_GSPath;
    string                          m_PSPath;

    string                          m_VSFuncName;
    string                          m_HSFuncName;
    string                          m_DSFuncName;
    string                          m_GSFuncName;
    string                          m_PSFuncName;

public:
    int CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName);
    int CreateHullShader(const wstring& _strRelativePath, const string& _strFuncName);
    int CreateDomainShader(const wstring& _strRelativePath, const string& _strFuncName);
    int CreateGeometryShader(const wstring& _strRelativePath, const string& _strFuncName);
    int CreatePixelShader(const wstring& _strRelativePath, const string& _strFuncName);

    void GetVSInfo(string& _RelativePath, string& _FuncName) const
    {
        _RelativePath = m_VSPath;
        _FuncName = m_VSFuncName;
    }

    void GetHSInfo(string& _RelativePath, string& _FuncName) const
    {
        _RelativePath = m_HSPath;
        _FuncName = m_HSFuncName;
    }

    void GetDSInfo(string& _RelativePath, string& _FuncName) const
    {
        _RelativePath = m_DSPath;
        _FuncName = m_DSFuncName;
    }

    void GetGSInfo(string& _RelativePath, string& _FuncName) const
    {
        _RelativePath = m_GSPath;
        _FuncName = m_GSFuncName;
    }

    void GetPSInfo(string& _RelativePath, string& _FuncName) const
    {
        _RelativePath = m_PSPath;
        _FuncName = m_PSFuncName;
    }

    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_Topology = _Topology; }
    void SetRSType(RS_TYPE _Type) { m_RSType = _Type; }
    void SetDSType(DS_TYPE _Type) { m_DSType = _Type; }
    void SetBSType(BS_TYPE _Type) { m_BSType = _Type; }
    void SetDomain(SHADER_DOMAIN _domain) { m_Domain = _domain; }

    D3D11_PRIMITIVE_TOPOLOGY GetTopology() const { return m_Topology; }
    RS_TYPE GetRSType() const { return m_RSType; }
    DS_TYPE GetDSType() const { return m_DSType; }
    BS_TYPE GetBSType() const { return m_BSType; }
    SHADER_DOMAIN GetDomain() const { return m_Domain; }



    void AddScalarParam(SCALAR_PARAM _Param, const string& _Desc, float _min = 0.f, float _Max = 0.f, bool _View = false, const string& _Tooltip = {})
    {
        m_ScalarParam[(UINT)_Param] = tScalarParam{ _Param , _Desc, _min, _Max, _View, _Tooltip, true };
    }
    void AddTexParam(TEX_PARAM _Param, const string& _Desc) { m_TexParam[(UINT)_Param] = tTexParam{ _Param, _Desc, true }; }

    void DeleteScalarParam(SCALAR_PARAM _Param) { m_ScalarParam[(UINT)_Param] = tScalarParam(); }
    void DeleteTexParam(TEX_PARAM _Param) { m_TexParam[(UINT)_Param] = tTexParam(); }

    const tScalarParam* GetScalarParam() { return  m_ScalarParam; }
    const tTexParam* GetTexParam() { return m_TexParam; }

    virtual int Save(const wstring& _strRelativePath) override;
    virtual int Load(const wstring& _strFilePath) override;
    virtual int Save(const string& _strRelativePath) override { return Save(ToWString(_strRelativePath)); }
    virtual int Load(const string& _strFilePath) override { return Load(ToWString(_strFilePath)); }

public:
    virtual int UpdateData();

public:
    CGraphicsShader();
    ~CGraphicsShader();
};

