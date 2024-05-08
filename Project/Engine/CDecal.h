#pragma once
#include "CRenderComponent.h"


class CDecal :
    public CRenderComponent
{
private:

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;

public:
    CLONE(CDecal);
    CDecal();
    ~CDecal();
};

