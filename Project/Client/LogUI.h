#pragma once
#include "UI.h"

class LogUI :
    public UI
{
private:

    UI* m_MessageUI;
    ImVec2 m_AvailableSize;
    ImVec2 m_UISize;

public:
    ImVec2 GetAvailableSize() { return m_AvailableSize; }
    ImVec2 GetUISize() { return m_UISize; }


public:
    virtual void tick() override;
    virtual void render_update() override;

public:
    LogUI();
    ~LogUI();
};

