#pragma once
#include "UI.h"

class LogUI :
    public UI
{
private:

    UI* m_MessageUI;

public:
    virtual void tick() override;
    virtual void render_update() override;

public:
    LogUI();
    ~LogUI();
};

