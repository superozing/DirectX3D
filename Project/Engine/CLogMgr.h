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
    vector<tLog> GetLogvec() { return m_vecLog; }
    void AddLog(Log_Level Lv, string msg);
    void AddLog(tLog Log);
    void AddTimeLog(Log_Level Lv, string msg);
    void CopyLog();
    void ClearLog();
};

