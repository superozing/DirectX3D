#pragma once
#include "CRenderComponent.h"

class CLandScape :
    public CRenderComponent
{
private:
    UINT            m_FaceX;
    UINT            m_FaceZ;

    Ptr<CTexture>   m_HeightMapTex;


public:
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMapTex = _HeightMap; }

    Ptr<CTexture> GetHeightMapTex() { return m_HeightMapTex; }
    void SetHeightMapTex(Ptr<CTexture> texture) { m_HeightMapTex = texture; }

    UINT GetLandScapeFaceX() { return m_FaceX; }
    UINT GetLandScapeFaceZ() { return m_FaceZ; }
    void SetLandScapeFace(UINT face, bool bXaxis);


private:
    void Init();
    void CreateMesh();

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

