﻿#pragma once
#include "UI.h"

class MessageUI :
    public UI
{
private:

    vector<tLog>    m_vectLog;

    int test;

public:
    virtual void tick() override;
    virtual void render_update() override;

public:
    MessageUI();
    ~MessageUI();
};

