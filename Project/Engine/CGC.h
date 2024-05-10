#pragma once
#include "CManager.h"

class CEntity;

class CGC :
    public CManager<CGC>
{
    SINGLE(CGC);
private:
    vector<CEntity*>    m_vecEntity;

public:
    virtual void tick() override;
    virtual void enter() override {}
    virtual void exit() override;

    void Add(CEntity* _Entity)
    {
        m_vecEntity.push_back(_Entity);
    }
};

