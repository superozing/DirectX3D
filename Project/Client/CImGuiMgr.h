#pragma once
#include <Engine/singleton.h>

class UI;

enum class FOCUS_STATE;

typedef void(UI::* Delegate_0)();
typedef void(UI::* Delegate_1)(DWORD_PTR);
typedef void(UI::* Delegate_2)(DWORD_PTR, DWORD_PTR);

typedef void (*CALL_BACK_0)(void);
typedef void (*CALL_BACK_1)(DWORD_PTR);
typedef void (*CALL_BACK_2)(DWORD_PTR, DWORD_PTR);

class CImGuiMgr :
    public CSingleton<CImGuiMgr>
{
    SINGLE(CImGuiMgr);
private:
    map<string, UI*>    m_mapUI;
    bool                m_bDemoUI;

    HANDLE              m_hNotify;

    static bool         isViewportFocused;

    Ptr<CTexture>       m_ViewPortTexture;

public:
    void init(HWND _hMainWnd, ComPtr<ID3D11Device> _Device, ComPtr <ID3D11DeviceContext> _Context);
    void progress();

public: // Callback
    static FOCUS_STATE GetFocus_debug();
    static FOCUS_STATE GetFocus_release();

public: // Viewport
    void CopyRTTex();
    Ptr<CTexture> GetViewportTexture() { return m_ViewPortTexture; }

private:
    void tick();
    void render();
    void create_ui();
    void observe_content();


public:
    UI* FindUI(const string& _strUIName);   
    void AddUI(const string& _strKey, UI* _UI);
};

