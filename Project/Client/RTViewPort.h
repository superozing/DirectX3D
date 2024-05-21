#pragma once
#include "UI.h"


class RTViewPort :
    public UI
{
private:
    CGameObject* m_pTarget;

    Ptr<CTexture>       m_ViewPortTexture;

public: 
    Ptr<CTexture> GetViewportTexture() { return m_ViewPortTexture; }

    void SetTarget(CGameObject* _target);

public:
    virtual void tick() override;
    virtual void render_update() override;

public:
    RTViewPort();
    ~RTViewPort();
};

