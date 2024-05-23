#pragma once
#include "ComponentUI.h"

class CLandScape;

class LandScapeUI :
    public ComponentUI
{
private:
    
    vector<string> m_vecHeightTextureKey;

public:
    virtual void render_update() override;
    virtual void ResetUIinfo() override;
    
    void GetLandScapeFileName();

public:
    LandScapeUI();
    ~LandScapeUI();
};

