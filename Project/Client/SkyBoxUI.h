#pragma once
#include "ComponentUI.h"

#include <Engine/CGameObject.h>

class SkyBoxUI :
    public ComponentUI
{
private:

    int     m_iSkyBoxTypeLv;

public:
    virtual void render_update() override;
public:
    SkyBoxUI();
    ~SkyBoxUI();
};

