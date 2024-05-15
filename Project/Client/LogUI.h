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
    char m_LogTextFilter[255];

public:
    ImVec2 GetAvailableSize() { return m_AvailableSize; }
    ImVec2 GetUISize() { return m_UISize; }

    bool* GetLogLvFilter() { return m_LoglvCheckBox; }
    char*  GetLogTextFilter() { return m_LogTextFilter; }


public:
    virtual void tick() override;
    virtual void render_update() override;

public:
    LogUI();
    ~LogUI();
};

