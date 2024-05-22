#pragma once
#include "ComponentUI.h"

#include <Engine/CGameObject.h>

class SkyBoxUI :
    public ComponentUI
{
private:

    int             m_iSkyBoxTypeLv;

    string          m_strTextureName[2];
    vector<string>  m_vecSkyBoxKey;

public:
    virtual void render_update() override;
    virtual void ResetUIinfo() override;;
    
    void GetSkyBoxFileName();

public:
    SkyBoxUI();
    ~SkyBoxUI();
};

