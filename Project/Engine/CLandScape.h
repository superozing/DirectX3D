#pragma once
#include "CRenderComponent.h"

#include "CHeightMapShader.h"
#include "CRaycastShader.h"
#include "CWeightMapShader.h"

class CLandScape :
    public CRenderComponent
{
private:
    UINT                    m_FaceX;
    UINT                    m_FaceZ;

    Vec4                    m_TessDivide;       // 각 edge, inside 분할 횟수(pow로 계산됨)
    Matrix                  m_matEdgeTessFactor;// landscape.fx의 GetTessFactor 함수에 들어감 // min lv, max lv, min distance, max distance

    Vec2                    m_BrushScale;       // 브러쉬 크기(전체 지형대비 크기 비율값)
    Ptr<CTexture>           m_BrushTex;         // 브러쉬용 텍스쳐
    float                   m_fBrushPow;

    Ptr<CHeightMapShader>   m_CSHeightMap;      // 높이맵 쉐이더
    bool                    m_bTessDir;
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

    Ptr<CTexture> GetHeightMapTex() { return m_HeightMapTex; }
    void SetHeightMapTex(Ptr<CTexture> texture) { m_HeightMapTex = texture; }

    Ptr<CTexture> GetBrushTex() { return m_BrushTex; }
    void SetBrushTex(Ptr<CTexture> texture) { m_BrushTex = texture; }

    Vec2 GetBrushScale() { return m_BrushScale; }
    void SetBrushScale(Vec2 BrushScale) { m_BrushScale = BrushScale; }

    float GetBrushPow() {return m_fBrushPow; }
    void SetBrushPow(float _Pow) { m_fBrushPow = _Pow; }

    void SetTessDir(bool bUp) { m_bTessDir = bUp; }

    UINT GetLandScapeFaceX() { return m_FaceX; }
    UINT GetLandScapeFaceZ() { return m_FaceZ; }
    void SetLandScapeFace(UINT face, bool bXaxis);

    Vec4 GetTessDivide() { return m_TessDivide; }
    void SetTessDivide(Vec4 _DevideParam) { m_TessDivide = _DevideParam; }

    Matrix GetEdgeTexFactor() {return m_matEdgeTessFactor;}
    void SetEdgeTexFactor(Matrix _mat) { m_matEdgeTessFactor = _mat; }

    LANDSCAPE_MODE GetLandScapeMode() { return m_Mode; }
    void SetLandScapeMode(LANDSCAPE_MODE _Mode) { m_Mode = _Mode; }

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

