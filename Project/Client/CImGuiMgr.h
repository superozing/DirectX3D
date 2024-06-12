#pragma once
#include <Engine/CManager.h>

class UI;

enum class FOCUS_STATE;

typedef void(UI::* Delegate_0)();
typedef void(UI::* Delegate_1)(DWORD_PTR);
typedef void(UI::* Delegate_2)(DWORD_PTR, DWORD_PTR);
typedef void(UI::* Delegate_3)(DWORD_PTR, UINT);

typedef void (*CALL_BACK_0)(void);
typedef void (*CALL_BACK_1)(DWORD_PTR);
typedef void (*CALL_BACK_2)(DWORD_PTR, DWORD_PTR);

enum class STATIC_BTN_TYPE
{
    TITLE,
    SUBTITLE,
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
    map<string, UI*>    m_mapUI;
    bool                m_bDemoUI;

    HANDLE              m_hNotify;

    static bool         isViewportFocused;

    vector<tStaticBtnColor>    m_vecStaticBtn;

    vector<string>      m_vecEnumTopology;
    vector<string>      m_vecEnumRS;
    vector<string>      m_vecEnumDS;
    vector<string>      m_vecEnumBS;
    vector<string>      m_vecEnumShaderDomain;


public:
    void init(HWND _hMainWnd, ComPtr<ID3D11Device> _Device, ComPtr <ID3D11DeviceContext> _Context);
    void begin();
    void progress();

    virtual void enter() override;

    static bool IsViewportFocused() { return isViewportFocused; }
    static void SetViewportFocused(bool _focus) { isViewportFocused = _focus; }

    const tStaticBtnColor& GetStaticButtonColor(STATIC_BTN_TYPE _type)
    {
        return m_vecStaticBtn[(UINT)_type];
    }

    const vector<string>& GetVecEnumTopology() { return m_vecEnumTopology; }
    const vector<string>& GetVecEnumRS() { return m_vecEnumRS; }
    const vector<string>& GetVecEnumDS() { return m_vecEnumDS; }
    const vector<string>& GetVecEnumBS() { return m_vecEnumBS; }
    const vector<string>& GetVecEnumShaderDomain() { return m_vecEnumShaderDomain; } 

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
};

