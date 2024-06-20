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
    Vec2         m_ViewportPos;
    float        m_fTapHeight;
    Vec2         m_MouseCoord;

private:
    void MoveCameraToObject();
    void Gizmo();

public: 
    Ptr<CTexture> GetViewportTexture() { return m_ViewPortTexture; }

    void SetTargetObject(CGameObject* _target);
    void SetTargetCamera(class CCamera* _camera);
    static void SetCamera(class CCamera* _camera);

    Vec2 GetViewPortPos() { return m_ViewportPos; }
    Vec2 GetViewPortSize() { return m_ViewportSize; }
    float GetTapHeight() { return m_fTapHeight; }
    Vec2 GetMouseCoord() { return m_MouseCoord; }
    static Vec2 ConvertCoord();

public:
    virtual void tick() override;
    virtual void render_update() override;
    virtual void enter() override;

public:
    RTViewPort();
    ~RTViewPort();
};

