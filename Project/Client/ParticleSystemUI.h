#pragma once
#include "ComponentUI.h"

#include <Engine\CParticleSystem.h>

class ParticleSystemUI :
    public ComponentUI
{
public:
    virtual void render_update() override;

public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

