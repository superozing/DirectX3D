#pragma once
#include "CAsset.h"


class CTexture :
    public CAsset
{
private:
    ScratchImage                        m_Image;    // 텍스쳐 로딩 및 시스템메모리 관리
    ComPtr<ID3D11Texture2D>             m_Tex2D;    // 텍스쳐 데이터를 GPU 메모리에 관리
    D3D11_TEXTURE2D_DESC                m_Desc;     // 텍스쳐 생성 정보

    ComPtr<ID3D11RenderTargetView>      m_RTV;      // 렌더타겟 용도
    ComPtr<ID3D11DepthStencilView>      m_DSV;      // 뎊스 스텐실 용도
    ComPtr<ID3D11ShaderResourceView>    m_SRV;      // 쉐이더에서 사용하는 용도(텍스쳐 레지스터(t) 바인딩)
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;      // GPGPU(General Purpose GPU) - ComputeShader, 읽기 쓰기 동시가능, (Unordered Register(u) 에 바인딩 가능)

    UINT                                m_RecentNum_SRV;
    UINT                                m_RecentNum_UAV;
    

private:
    virtual int Save(const wstring& _strRelativePath) override;

    int Load(const wstring& _strFilePath, int _iMipLevel);
    virtual int Load(const wstring& _strFilePath) override;

    int Create(UINT _Width, UINT _Height
        , DXGI_FORMAT _Format, UINT _BindFlag
        , D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
    int Create(ComPtr<ID3D11Texture2D> _tex2D);
    int CreateArrayTexture(const vector<Ptr<CTexture>>& _vecTex, int _iMapLevel);

public:
    void GenerateMip(UINT _iMipLevel);

public:
    void UpdateData(int _RegisterNum);

    int UpdateData_CS_SRV(int _RegisterNum);
    int UpdateData_CS_UAV(int _RegisterNum);

    static void Clear(int _RegisterNum);
    void Clear_CS_SRV();
    void Clear_CS_UAV();

    UINT    GetWidth() { return m_Desc.Width;}
    UINT    GetHeight() { return m_Desc.Height; }

    UINT    GetArraySize() { return m_Desc.ArraySize; }
    UINT    GetRowPitch() const { return (UINT)m_Image.GetImages()->rowPitch; }
    UINT    GetSlicePitch() const { return (UINT)m_Image.GetImages()->slicePitch; }
    void*   GetSysMem() { return m_Image.GetPixels(); }

    ComPtr<ID3D11Texture2D>           GetTex2D() { return m_Tex2D; }
	ComPtr<ID3D11RenderTargetView>    GetRTV() { return m_RTV; }
	ComPtr<ID3D11DepthStencilView>    GetDSV() { return m_DSV; }
	ComPtr<ID3D11ShaderResourceView>  GetSRV() { return m_SRV; }
	ComPtr<ID3D11UnorderedAccessView> GetUAV() { return m_UAV; }
    const D3D11_TEXTURE2D_DESC&       GetDesc() { return m_Desc; }
    tPixel* GetPixels();

    CLONE_DISABLE(CTexture);
public:
    CTexture(bool _Engine = false);
    ~CTexture();

    friend class CAssetMgr;
};

