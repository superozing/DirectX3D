#include "pch.h"
#include "LogUI.h"

#include <Engine\CLogMgr.h>

#include "MessageUI.h"


LogUI::LogUI()
	: UI("Log", "##LogUI")
{
    m_MessageUI = new MessageUI;
    AddChildUI(m_MessageUI);

    m_LoglvCheckBox[0] = true;
    m_LoglvCheckBox[1] = false;
    m_LoglvCheckBox[2] = false;
    m_LoglvCheckBox[3] = false;



}

LogUI::~LogUI()
{
}

void LogUI::tick()
{
}

void LogUI::render_update()
{
    ImVec4 Red = ImVec4(1.f, 0.f, 0.f, 1.0f);
    ImVec4 Green = ImVec4(0.f, 1.f, 0.f, 1.0f);
    ImVec4 Blue = ImVec4(0.f, 0.f, 1.f, 1.0f);
    ImVec4 Gray = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_Button, Gray); // 기본 배경색
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Red); // 호버 배경색도 기본 배경색과 동일하게 설정
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, Red);
    if (ImGui::Button("CopyLog"))
    {
        CLogMgr::GetInst()->CopyLog();
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, Gray); // 기본 배경색
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Red); // 호버 배경색도 기본 배경색과 동일하게 설정
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, Red);
    if (ImGui::Button("DeleteLog"))
    {
        CLogMgr::GetInst()->ClearLog();
    }
    ImGui::PopStyleColor(3);

    if (ImGui::Checkbox("All", &m_LoglvCheckBox[0]))
    {
        if (m_LoglvCheckBox[1] || m_LoglvCheckBox[2] || m_LoglvCheckBox[3])
        {
            m_LoglvCheckBox[1] = false;
            m_LoglvCheckBox[2] = false;
            m_LoglvCheckBox[3] = false;
        }
    }
    ImGui::SameLine();

    
    if (ImGui::Checkbox("Info", &m_LoglvCheckBox[1]))
    {
        m_LoglvCheckBox[0] = false;
    }
    ImGui::SameLine();

    if (ImGui::Checkbox("Error", &m_LoglvCheckBox[2]))
    {
        m_LoglvCheckBox[0] = false;
    }
    ImGui::SameLine();

    if (ImGui::Checkbox("Warn", &m_LoglvCheckBox[3]))
    {
        m_LoglvCheckBox[0] = false;
    }

    ImGui::Text("Search");
    ImGui::SameLine();

    ImGui::PushItemWidth(-FLT_MIN);
    ImGui::InputText("##Search", m_LogTextFilter, IM_ARRAYSIZE(m_LogTextFilter));
    ImGui::PopItemWidth();

    m_AvailableSize = ImGui::GetContentRegionAvail();
    m_UISize = ImGui::GetWindowSize();
    
    ImGui::Separator();
}