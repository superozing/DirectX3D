#pragma once
#include "UI.h"

class LogUI :
    public UI
{
private:

    UI* m_MessageUI;
    UI* m_FunctionUI;

public:
    virtual void tick() override;
    virtual void render_update() override;

public:
    LogUI();
    ~LogUI();
};

