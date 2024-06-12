#pragma once
#include <Engine/CUIScript.h>

class CPanelUIScript :
    public CUIScript
{
private:
    Vec2            m_vLbtnDownPos;

    Ptr<CTexture>   m_PanelTex;
    bool            m_AllowTexSet;

    bool            m_AllowDragAndDrop;

public:
    virtual void tick() override;

public:
    virtual void LBtnDown() override;

public:
    CLONE(CPanelUIScript);
    CPanelUIScript();
    CPanelUIScript(const CPanelUIScript& _Other);
    ~CPanelUIScript();
};