#pragma once
#include "ComponentUI.h"

class CParticleSystem;

class ParticleSystemUI :
    public ComponentUI
{
private:

    vector<string>  m_vecParticleKey;

public:
    virtual void render_update() override;
    virtual void ResetUIinfo() override;
    
    void GetParticleFileName();


public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

