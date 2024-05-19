﻿#pragma once
#include "UI.h"

class LogUI :
    public UI
{
private:

    vector<tLog>    m_vectLog;
    ImVec4          m_LogColor[(UINT)Log_Level::END];
    
    ImVec2 m_AvailableSize;

    int             m_iLoglvMask;
    int             m_iLogTimeMask;
    ImGuiTextFilter m_TextFilter;


    bool m_LoglvCheckBox[4];
    bool m_LogTimePrint[4];
    char m_LogTextFilter[255];

    float           m_fLastScrollY;

public:
    ImVec4 SetLogColor(tLog Log);
    void CheckLevelMask();
    void CheckTimeMask();
    bool CheckSearchDisplay(string msg);
    bool CheckLogLvDispaly(Log_Level Loglv);
    string GetTimeMsg(double time);
    bool* GetLogLvFilter() { return m_LoglvCheckBox; }
    char*  GetLogTextFilter() { return m_LogTextFilter; }


public:
    virtual void tick() override;
    virtual void render_update() override;

public:
    LogUI();
    ~LogUI();
};

