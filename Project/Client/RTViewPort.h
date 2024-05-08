#pragma once
#include "UI.h"


class RTViewPort :
    public UI
{
private:
    

    Ptr<CTexture>       m_ViewPortTexture;

public: 
    void CopyRTTex();
    Ptr<CTexture> GetViewportTexture() { return m_ViewPortTexture; }

public:
    virtual void tick() override;
    virtual void render_update() override;

public:
    RTViewPort();
    ~RTViewPort();
};

