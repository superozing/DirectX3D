#pragma once
#include "CComputeShader.h"
class CWeightMapShader :
    public CComputeShader
{
private:
    CStructuredBuffer* m_WeightMap;
    CStructuredBuffer* m_RaycastInfo;

    Ptr<CTexture>      m_BrushArrTex;
    Vec2               m_BrushScale;
    int                m_BrushIdx;
    float              m_fBrushPow;
    int                m_WeightIdx;

    int                m_Width;
    int                m_Height;

public:
    void SetWeightMap(CStructuredBuffer* _pWeightMap, UINT _iWidth, UINT _iHeight)
    {
        m_WeightMap = _pWeightMap;
        m_Width = _iWidth;
        m_Height = _iHeight;
    }

    void SetInputBuffer(CStructuredBuffer* _pRaycastData) { m_RaycastInfo = _pRaycastData; }
    void SetBrushArrTex(Ptr<CTexture> _pBrushTex) { m_BrushArrTex = _pBrushTex; }
    void SetBrushScale(Vec2 _vScale) { m_BrushScale = _vScale; }
    void SetBrushIndex(int _iIdx) { m_BrushIdx = _iIdx; }
    void SetBrushPow(float _Pow) { m_fBrushPow = _Pow; }
    void SetWeightIdx(int _iIdx) { m_WeightIdx = _iIdx; }

public:
    virtual int UpdateData() override;
    virtual void UpdateGroupCount() override;
    virtual void Clear() override;


public:
    CWeightMapShader();
    ~CWeightMapShader();
};

