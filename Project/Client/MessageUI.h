#pragma once
#include "UI.h"

class MessageUI :
    public UI
{
private:

    vector<tLog>    m_vectLog;
    ImVec4          m_LogColor[(UINT)Log_Level::END];

public:
    ImVec4 SetLogColor(tLog Log);

public:
    virtual void tick() override;
    virtual void render_update() override;

public:
    MessageUI();
    ~MessageUI();
};

