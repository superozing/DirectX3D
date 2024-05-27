#pragma once
#include "CComputeShader.h"

#include "ptr.h"
#include "CTexture.h"

class CStructuredBuffer;

class CHeightMapShader :
    public CComputeShader
{
private:
    Ptr<CTexture>       m_pHeightMap;
    Ptr<CTexture>       m_pBrushTex;
    Vec2                m_vScale;       // 높이 Brush 크기
    int                 m_iBrushIdx;
    float               m_fBrushPow;
    CStructuredBuffer* m_pInput;       // Ray 충돌 위치

    int                 m_iUpdown;  // 상승, 하강 방향 설정

public:
    void SetHeightMap(Ptr<CTexture> _pTex) { m_pHeightMap = _pTex; }
   
    void SetBrushTex(Ptr<CTexture> _pTex) { m_pBrushTex = _pTex; }
    void SetBrushScale(Vec2 _vScale) { m_vScale = _vScale; }
    void SetBrushIndex(int _iIdx) { m_iBrushIdx = _iIdx; }
    void SetBrushPow(float _Pow) { m_fBrushPow = _Pow; }

    void SetInputBuffer(CStructuredBuffer* _pInput) { m_pInput = _pInput; }

    void SetTesDir(bool bUp) { m_iUpdown = (int)bUp; }


public:
    virtual int UpdateData() override;
    virtual void UpdateGroupCount() override;
    virtual void Clear() override;


public:
    CHeightMapShader();
    ~CHeightMapShader();
};