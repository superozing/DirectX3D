#pragma once
#include "ComponentUI.h"

class CLandScape;


class LandScapeUI :
    public ComponentUI
{
private:
    
    vector<string> m_vecHeightTextureKey;
    vector<string> m_vecBrushTextureKey;
    vector<string> m_vecLanderScapeMode;

    Matrix m_matTessFactor;


    Ptr<CMaterial> m_pTargetObjMtrl;


public:
    virtual void render_update() override;
    virtual void ResetUIinfo() override;
    
    void GetLandScapeFileName();

public:
    LandScapeUI();
    ~LandScapeUI();
};

