#pragma once
#include "ComponentUI.h"
class PhysXUI :
    public ComponentUI
{
private:
public:
    virtual void render_update() override;

public:
    PhysXUI();
    ~PhysXUI();
};


