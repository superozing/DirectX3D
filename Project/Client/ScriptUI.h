#pragma once
#include "ComponentUI.h"

#include <Engine/CScript.h>

class ScriptUI :
    public ComponentUI
{
private:
    CScript* m_TargetScript;

public:
    void SetScript(CScript* _Script);
    CScript* GetTargetScript() { return m_TargetScript; }

    virtual void render_update() override;


public:
    ScriptUI();
    ~ScriptUI();
};

