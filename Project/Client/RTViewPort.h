#pragma once
#include "UI.h"

class RTViewPort :
    public UI
{
private:
    CGameObject* m_pTarget;
    CGameObject* m_pCamera;

    Ptr<CTexture>       m_ViewPortTexture;

    Vec2         m_ViewportSize;
    Vec2         m_LClickedCoord;

private:
    void MoveCameraToObject();
    void Gizmo();

public: 
    Ptr<CTexture> GetViewportTexture() { return m_ViewPortTexture; }

    void SetTargetObject(CGameObject* _target);
    void SetTargetCamera(class CCamera* _camera);
    static void SetCamera(class CCamera* _camera);

    Vec2 GetViewPortSize() { return m_ViewportSize; }
    Vec2 GetLClickCoord() { return m_LClickedCoord; }
    static Vec2 ConvertCoord();

public:
    virtual void tick() override;
    virtual void render_update() override;

public:
    RTViewPort();
    ~RTViewPort();
};

