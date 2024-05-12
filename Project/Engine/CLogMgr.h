#pragma once
#include "CManager.h"

class CLogMgr :
    public CManager<CLogMgr>
{
    SINGLE(CLogMgr)
};

