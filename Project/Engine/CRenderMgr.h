#pragma once
#include "singleton.h"

#include "CTexture.h"

class CGameObject;
class CCamera;
class CLight2D;
class CLight3D;
class CStructuredBuffer;
class CMRT;


class CRenderMgr :
    public CManager<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    CMRT*                   m_arrMRT[(UINT)MRT_TYPE::END];

    vector<CCamera*>        m_vecCam;
    CCamera* m_EditorCam;
    bool    m_bEscape;

    Ptr<CTexture>           m_PostProcessTex;

    CStructuredBuffer*      m_Light2DBuffer;
    vector<CLight2D*>       m_vecLight2D;

    CStructuredBuffer*      m_Light3DBuffer;
    vector<CLight3D*>       m_vecLight3D;

    list<tDebugShapeInfo>   m_DbgShapeInfo;
    CGameObject* m_pDebugObj;
    bool                    m_DebugPosition;

    // NoiseTexture
    vector<Ptr<CTexture>>   m_vecNoiseTex;

    // render function pointer
    typedef void(CRenderMgr::* RENDER_FUNC)(void);
    RENDER_FUNC             m_RenderFunc;

    Vec4 m_vClearColor;


public:
    static void(*CameraChange)(CCamera*);
    /// <summary>
    /// 0번 idx == MainCamera
    /// </summary>
    void RegisterCamera(CCamera* _Cam, int _Idx);
    void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_DbgShapeInfo.push_back(_info); }

    void SetDebugPosition(bool _OnOff) { m_DebugPosition = _OnOff; }
    bool IsDebugPosition() { return m_DebugPosition; }

    void RegisterLight2D(CLight2D* _Light2D) { m_vecLight2D.push_back(_Light2D); }
    int RegisterLight3D(CLight3D* _Light3D) 
    { 
        m_vecLight3D.push_back(_Light3D); 
        return m_vecLight3D.size() - 1; 
    }


    void CopyRenderTargetToPostProcessTarget();
    Ptr<CTexture> GetPostProcessTex() { return m_PostProcessTex; }

    Ptr<CTexture> CopyRTTex(Ptr<CTexture> pTexture);

    void RegisterEditorCamera(CCamera* _Cam) { 
        m_EditorCam = _Cam;
        if (CameraChange) CameraChange(m_EditorCam);
    }
    void ClearCamera() { m_vecCam.clear(); }

    void ActiveEditorMode(bool _bActive)
    {
        if (_bActive)
            m_RenderFunc = &CRenderMgr::render_editor;
        else
            m_RenderFunc = &CRenderMgr::render_play;
    }

    CMRT* GetMRT(MRT_TYPE _Type) { return m_arrMRT[(UINT)_Type]; }

    const vector<CLight3D*>& GetLight3D() { return m_vecLight3D; }

    void SetClearColor(const Vec4& _ClearColor) { m_vClearColor = _ClearColor; }
    Vec4 GetClearColor() { return m_vClearColor;}

    void CheckEscape();

    CCamera* GetMainCam();
    CCamera* GetEditorCam() { return m_EditorCam; }

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void enter() override {}
    virtual void exit() override;

private:

    void CreateDynamicShadowDepth();

    void CreateMRT();
    void ClearMRT();

    void ResetMRT();

    void render_play();
    void render_editor();

    void render_debug();

    // 리소스 바인딩
    void UpdateData();

    // 리소스 클리어
    void Clear();

    friend class CRenderMgrScript;
	friend class CDevice;
};

