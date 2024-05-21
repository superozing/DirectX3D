#pragma once
#include "ComponentUI.h"

class CParticleSystem;

class ParticleSystemUI :
    public ComponentUI
{
private:
    CParticleSystem* m_TargetParticleComponent;
    Ptr<CTexture>    m_ParticleTexture;

public:
    virtual void render_update() override;

public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

