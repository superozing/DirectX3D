#pragma once
#include "ComponentUI.h"

#include <Engine/CGameObject.h>

class SkyBoxUI :
    public ComponentUI
{
public:
    virtual void render_update() override;
public:
    SkyBoxUI();
    ~SkyBoxUI();
};

