#pragma once
#include "UI.h"

class LogUI :
    public UI
{
private:

    UI* m_MessageUI;
    ImVec2 m_AvailableSize;
    ImVec2 m_UISize;

    bool m_LoglvCheckBox[4];

public:
    ImVec2 GetAvailableSize() { return m_AvailableSize; }
    ImVec2 GetUISize() { return m_UISize; }

    bool* GetLogLvFilter() { return m_LoglvCheckBox; }


public:
    virtual void tick() override;
    virtual void render_update() override;

public:
    LogUI();
    ~LogUI();
};

