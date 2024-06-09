#pragma once
#include "AssetUI.h"

#include <Engine/CGameObject.h>

class PrefabUI :
    public AssetUI
{
private:
    CGameObject* m_TargetObject;

public:
    virtual void render_update() override;

public:
    PrefabUI();
    ~PrefabUI();
};

