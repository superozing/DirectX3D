 #pragma once
#include "CManager.h"

class CLogMgr :
    public CManager<CLogMgr>
{
    SINGLE(CLogMgr)

private:

    vector<tLog> m_vecLog;

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

    string GetTimeMsg(double time);
    void CopyLog();
    void ClearLog();
};

