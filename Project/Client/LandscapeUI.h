﻿#pragma once
#include "ComponentUI.h"

class CLandScape;

class LandScapeUI :
    public ComponentUI
{
private:
    
    vector<string> m_vecHeightTextureKey;
    vector<string> m_vecBrushTextureKey;

    Ptr<CMaterial> m_pTargetObjMtrl;


public:
    virtual void render_update() override;
    virtual void ResetUIinfo() override;
    
    void GetLandScapeFileName();
    void SetChangeTessFactor(Vec4* _mtrlparam, float* changevalue);

public:
    LandScapeUI();
    ~LandScapeUI();
};

