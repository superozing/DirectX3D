#pragma once
#include "ComponentUI.h"

class CParticleSystem;

class ParticleSystemUI :
    public ComponentUI
{
private:

    vector<string>  m_vecParticleKey;
    int             m_iNewMaxCount;

    ImVec4          m_ModuleButtonColor[2];

public:
    virtual void render_update() override;
    virtual void ResetUIinfo() override;
    
    void GetParticleFileName();

    void PayloadParticleEvent(CAsset* _Asset, ASSET_TYPE _Type);

public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

