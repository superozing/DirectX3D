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


private:
    void Init();
    void CreateMesh();

public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void UpdateData() override;

public:
    CLONE(CLandScape);
    CLandScape();
    ~CLandScape();
};

