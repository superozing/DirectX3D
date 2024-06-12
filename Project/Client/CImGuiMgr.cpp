#include "pch.h"
#include "CImGuiMgr.h"

#include <Engine/CEngine.h>
#include <Engine/define.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>

#include <Engine/CRenderMgr.h>
#include <Engine/CPathMgr.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Inspector.h"
#include "ComponentUI.h"
#include "Content.h"
#include "Outliner.h"
#include "MenuUI.h"
#include "ListUI.h"
#include "RTViewPort.h"
#include "LogUI.h"

#include "ParamUI.h"

bool CImGuiMgr::isViewportFocused;

CImGuiMgr::CImGuiMgr()
    : m_bDemoUI(false)
    , m_hNotify(nullptr)
{

}

CImGuiMgr::~CImGuiMgr()
{
    // ImGui Clear       
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // UI 
    Delete_Map(m_mapUI);

    // ���͸� ���� ���� ����
    FindCloseChangeNotification(m_hNotify);
}

void CImGuiMgr::init(HWND _hMainWnd, ComPtr<ID3D11Device> _Device
    , ComPtr<ID3D11DeviceContext> _Context)
{
    // ImGui �ʱ�ȭ
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    StyleColorPupple();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    //ImGuiStyle& style = ImGui::GetStyle();
    //if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    //{
    //    style.WindowRounding = 0.0f;
    //    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    //}

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_hMainWnd);
    ImGui_ImplDX11_Init(_Device.Get(), _Context.Get());

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Consolas.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Load Font(Consolas)
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 14.0f, NULL, io.Fonts->GetGlyphRangesKorean());

    create_ui();
    
    // Content ���� ����
    wstring strContentPath = CPathMgr::GetContentPath();
    m_hNotify = FindFirstChangeNotification(strContentPath.c_str(), true
                                        , FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME 
                                        | FILE_ACTION_ADDED | FILE_ACTION_REMOVED);


    // static button init
    
    m_vecStaticBtn.resize((UINT)STATIC_BTN_TYPE::END);

    tStaticBtnColor tColor{};

    tColor.ColBtnColor = (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f);
    tColor.ColBtnHoveredColor = (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f);
    tColor.ColBtnActiveColor = (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f);

    m_vecStaticBtn[(UINT)STATIC_BTN_TYPE::TITLE] = tColor;


    // Enum String Init
    m_vecEnumTopology.resize((UINT)D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST);
    for (UINT i = 0; i < m_vecEnumTopology.size(); ++i)
        m_vecEnumTopology[i] = ToString(magic_enum::enum_name((D3D11_PRIMITIVE_TOPOLOGY)i));

    m_vecEnumRS.resize((UINT)RS_TYPE::END - 1);
    for (UINT i = 0; i < m_vecEnumRS.size(); ++i)
        m_vecEnumRS[i] = ToString(magic_enum::enum_name((RS_TYPE)i));

    m_vecEnumDS.resize((UINT)DS_TYPE::END - 1);
    for (UINT i = 0; i < m_vecEnumDS.size(); ++i)
        m_vecEnumDS[i] = ToString(magic_enum::enum_name((DS_TYPE)i));

    m_vecEnumBS.resize((UINT)BS_TYPE::END - 1);
    for (UINT i = 0; i < m_vecEnumBS.size(); ++i)
        m_vecEnumBS[i] = ToString(magic_enum::enum_name((BS_TYPE)i));

    m_vecEnumShaderDomain.resize((UINT)SHADER_DOMAIN::DOMAIN_DEBUG);
    for (UINT i = 0; i < m_vecEnumShaderDomain.size(); ++i)
        m_vecEnumShaderDomain[i] = ToString(magic_enum::enum_name((SHADER_DOMAIN)i));

    tColor.ColBtnColor = (ImVec4)ImColor::HSV(0.9f, 0.8f, 0.8f);
    tColor.ColBtnHoveredColor = (ImVec4)ImColor::HSV(0.9f, 0.8f, 0.8f);
    tColor.ColBtnActiveColor = (ImVec4)ImColor::HSV(0.9f, 0.8f, 0.8f);

    m_vecStaticBtn[(UINT)STATIC_BTN_TYPE::SUBTITLE] = tColor;


    RTViewPort* pViewport = (RTViewPort*)CImGuiMgr::GetInst()->FindUI("##Viewport");
    pViewport->SetCamera(CRenderMgr::GetInst()->GetEditorCam());
}

void CImGuiMgr::progress()
{
    tick();

    render();

    observe_content();
}

void CImGuiMgr::enter()
{
    for (auto iter = m_mapUI.begin(); iter != m_mapUI.end(); ++iter) {
        iter->second->enter();
    }
}

FOCUS_STATE CImGuiMgr::GetFocus_debug()
{    
    // 현재 포커싱 된 창이 없을 경우
    if (GetFocus() == nullptr)
    {
        return FOCUS_STATE::NONE;
    }

    // 현재 포커싱이 뷰포트일 경우
    if (GetFocus() == CEngine::GetInst()->GetMainWind() && IsViewportFocused())
    {
        isViewportFocused = false;
        return FOCUS_STATE::MAIN;
    }

    // 뷰포트가 아닌 다른 ImGui 창이 포커싱 된 경우
    else
    {
        return FOCUS_STATE::OTHER;
    }
    
}

FOCUS_STATE CImGuiMgr::GetFocus_release()
{
    if (GetFocus() == CEngine::GetInst()->GetMainWind())
    {
        return FOCUS_STATE::MAIN;
    }
    else
    {
        return FOCUS_STATE::NONE;
    }
}

void CImGuiMgr::begin()
{
    Outliner* pOutlinerUI = (Outliner*)FindUI("##Outliner");

    if (pOutlinerUI)
    {
        pOutlinerUI->ResetCurrentLevel();
    }
    else
    {
        MessageBox(nullptr, L"Outliner 초기화 실패", L"초기화 실패", MB_OK);
    }
}

void CImGuiMgr::tick()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    if (m_bDemoUI)
    {
        ImGui::ShowDemoWindow(&m_bDemoUI);
    }

    for (const auto& pair : m_mapUI)
    {
        pair.second->tick();
    }

    ParamUI::ResetID();
}

void CImGuiMgr::render()
{

    for (const auto& pair : m_mapUI)
    {
        pair.second->render();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}


UI* CImGuiMgr::FindUI(const string& _strUIName)
{
    map<string, UI*>::iterator iter = m_mapUI.find(_strUIName);

    if (iter == m_mapUI.end())
        return nullptr;

    return iter->second;
}

void CImGuiMgr::AddUI(const string& _strKey, UI* _UI)
{
    UI* pUI = FindUI(_strKey);
    assert(!pUI);
    m_mapUI.insert(make_pair(_strKey, _UI));
}

#include <Engine\CLevelMgr.h>
void CImGuiMgr::create_ui()
{
    UI* pUI = nullptr;

    // Inspector
    pUI = new Inspector;
    AddUI(pUI->GetID(), pUI);
    CLevelMgr::GetInst()->RegisterClientFunction(Client_Function_Type::CIMGUIMGR_ENTER, std::bind(&CImGuiMgr::enter, this));

    // Content
    pUI = new Content;
    AddUI(pUI->GetID(), pUI);

    // Outliner
    pUI = new Outliner;
    AddUI(pUI->GetID(), pUI);

    // MenuUI
    pUI = new MenuUI;
    AddUI(pUI->GetID(), pUI);

    // List
    pUI = new ListUI;
    AddUI(pUI->GetID(), pUI);

    // Viewport
    pUI = new RTViewPort;
    AddUI(pUI->GetID(), pUI);

    // Log
    pUI = new LogUI;
    AddUI(pUI->GetID(), pUI);
}

void CImGuiMgr::observe_content()
{
    // WaitForSingleObject �� �̿��ؼ� �˸��� �ִ��� Ȯ��,
    // ���ð��� 0�� �����ؼ� �˸��� �ִ� ���� �ٷ� ��ȯ
    if (WAIT_OBJECT_0 == WaitForSingleObject(m_hNotify, 0))
    {
        // �ٽ� �˸� Ȱ��ȭ
        FindNextChangeNotification(m_hNotify);

        // ContentUI �� Reload �۾� ����
        Content* pContentUI = (Content*)FindUI("##Content");
        pContentUI->ResetContent();

        // InspectorUI Reload
        Inspector* pInspectorUI = (Inspector*)FindUI("##Inspector");
        pInspectorUI->ResetComponent();
       
      
    }
}

void CImGuiMgr::StyleColorPupple()
{
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 1.0f };
    colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

    // Border
    colors[ImGuiCol_Border] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.29f };
    colors[ImGuiCol_BorderShadow] = ImVec4{ 0.0f, 0.0f, 0.0f, 0.24f };

    // Text
    colors[ImGuiCol_Text] = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };
    colors[ImGuiCol_TextDisabled] = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.13f, 0.13f, 0.17, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.13f, 0.13f, 0.17, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
    colors[ImGuiCol_CheckMark] = ImVec4{ 0.74f, 0.58f, 0.98f, 1.0f };

    // Popups
    colors[ImGuiCol_PopupBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 0.92f };

    // Slider
    colors[ImGuiCol_SliderGrab] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.54f };
    colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.74f, 0.58f, 0.98f, 0.54f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.13f, 0.13, 0.17, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.24, 0.24f, 0.32f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.2f, 0.22f, 0.27f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 1.0f };
    colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{ 0.24f, 0.24f, 0.32f, 1.0f };

    // Seperator
    colors[ImGuiCol_Separator] = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };
    colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.74f, 0.58f, 0.98f, 1.0f };
    colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.84f, 0.58f, 1.0f, 1.0f };

    // Resize Grip
    colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.29f };
    colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.74f, 0.58f, 0.98f, 0.29f };
    colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.84f, 0.58f, 1.0f, 0.29f };

    // Docking
    colors[ImGuiCol_DockingPreview] = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };

    auto& style = ImGui::GetStyle();
    style.TabRounding = 4;
    style.ScrollbarRounding = 9;
    style.WindowRounding = 7;
    style.GrabRounding = 3;
    style.FrameRounding = 3;
    style.PopupRounding = 4;
    style.ChildRounding = 4;
}






