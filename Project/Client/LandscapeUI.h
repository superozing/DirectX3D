#pragma once
#include "ComponentUI.h"

class CLandScape;

class LandScapeUI :
    public ComponentUI
{

public:
    virtual void render_update() override;

public:
    LandScapeUI();
    ~LandScapeUI();
};

