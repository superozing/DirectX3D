#pragma once
#include "ComponentUI.h"

class CParticleSystem;

class ParticleSystemUI :
    public ComponentUI
{
private:

    tParticleModule* m_TargetParticleModule;

public:
    virtual void render_update() override;

public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

