#pragma once
#include "UI.h"


class RTViewPort :
    public UI
{
public:
    virtual void tick() override;
    virtual void render_update() override;

public:
    RTViewPort();
    ~RTViewPort();
};

