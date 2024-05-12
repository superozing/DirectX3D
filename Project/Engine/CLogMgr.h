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
    void AddLog();
    void CopyLog();
    void ClearLog();
};

