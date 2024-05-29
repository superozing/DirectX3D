#pragma once
#include "CRenderComponent.h"

#include "CHeightMapShader.h"
#include "CRaycastShader.h"
#include "CWeightMapShader.h"

enum class LANDSCAPE_MODE
{
    HEIGHT_MAP,
    SPLAT,
    NONE,
};


class CLandScape :
    public CRenderComponent
{
private:
    UINT                    m_FaceX;
    UINT                    m_FaceZ;

    Vec2                    m_BrushScale;       // 브러쉬 크기(전체 지형대비 크기 비율값)
    Ptr<CTexture>           m_BrushTex;         // 브러쉬용 텍스쳐

    Ptr<CHeightMapShader>   m_CSHeightMap;      // 높이맵 쉐이더
    Ptr<CTexture>           m_HeightMapTex;     // 높이맵

    Ptr<CRaycastShader>     m_CSRaycast;        // 픽킹 쉐이더
    CStructuredBuffer*      m_CrossBuffer;	    // 마우스 피킹되는 지점 정보 받는 버퍼

    Ptr<CWeightMapShader>   m_CSWeightMap;      // 가중치 쉐이더
    CStructuredBuffer*      m_WeightMapBuffer;  // 가중치 저장 버퍼
    UINT                    m_WeightWidth;      // 가중치 버퍼 가로세로 행렬 수
    UINT                    m_WeightHeight;     // 가중치 버퍼 가로세로 행렬 수
    UINT                    m_WeightIdx;        // 증가시킬 가중치 부위

    Ptr<CTexture>           m_TileArrTex;       // 타일 배열 텍스쳐

    LANDSCAPE_MODE          m_Mode;

public:
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMapTex = _HeightMap; }
    void Raycasting();

private:
    void Init();
    void CreateMesh();
    void CreateComputeShader();
    void CreateTexture();

public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void UpdateData() override;

    virtual void SaveToFile(ofstream& fout) override;
    virtual void LoadFromFile(ifstream& fin) override;

public:
    CLONE(CLandScape);
    CLandScape();
    ~CLandScape();
};

