 #pragma once
#include "CManager.h"

class CLogMgr :
    public CManager<CLogMgr>
{
    SINGLE(CLogMgr)

private:

    vector<tLog> m_vecLog;
    int          m_LogTimeMask;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void enter() override;
    virtual void exit() override;

public:
    vector<tLog>& GetLogs() { return m_vecLog; }
    void AddLog(Log_Level Lv, string msg);
    void AddLog(Log_Level Lv, wstring msg);
    void AddLog(tLog Log);

    int GetTimeMask() { return m_LogTimeMask; }
    void SetTimeMask(int iMask) { m_LogTimeMask = iMask; }
    string GetTimeMsg(double time, int TimeMask);
    void CopyLog();
    void ClearLog();
};

