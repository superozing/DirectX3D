#pragma once
#include <Engine/CManager.h>

class UI;

enum class FOCUS_STATE;

typedef void(UI::* Delegate_0)();
typedef void(UI::* Delegate_1)(DWORD_PTR);
typedef void(UI::* Delegate_2)(DWORD_PTR, DWORD_PTR);

typedef void (*CALL_BACK_0)(void);
typedef void (*CALL_BACK_1)(DWORD_PTR);
typedef void (*CALL_BACK_2)(DWORD_PTR, DWORD_PTR);

enum class STATIC_BTN_TYPE
{
    TITLE,

    END
};

struct tStaticBtnColor
{
    ImVec4 ColBtnColor;
    ImVec4 ColBtnHoveredColor;
    ImVec4 ColBtnActiveColor;
};

class CImGuiMgr :
    public CManager<CImGuiMgr>
{
    SINGLE(CImGuiMgr);
private:
    vector<tStaticBtnColor>    m_vecStaticBtn;

    map<string, UI*>    m_mapUI;
    bool                m_bDemoUI;

    HANDLE              m_hNotify;

    static bool         isViewportFocused;

public:
    void init(HWND _hMainWnd, ComPtr<ID3D11Device> _Device, ComPtr <ID3D11DeviceContext> _Context);
    void begin();
    void progress();

    virtual void enter() override;

    bool& GetbViewportFocused() { return isViewportFocused; }

    const tStaticBtnColor& GetStaticButtonColor(STATIC_BTN_TYPE _type)
    {
        return m_vecStaticBtn[(UINT)_type];
    }

public: // Callback
    static FOCUS_STATE GetFocus_debug();
    static FOCUS_STATE GetFocus_release();

private:
    void tick();
    void render();
    void create_ui();
    void observe_content();

    void StyleColorPupple();

public:
    UI* FindUI(const string& _strUIName);   
    void AddUI(const string& _strKey, UI* _UI);

private:
    void ResetInspectorTarget();
};

