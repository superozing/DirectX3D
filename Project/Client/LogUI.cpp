#include "pch.h"
#include "LogUI.h"

#include "MessageUI.h"


LogUI::LogUI()
	: UI("Log", "##LogUI")
{
    m_MessageUI = new MessageUI;
    AddChildUI(m_MessageUI);

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
       // 
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, Gray); // 기본 배경색
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Red); // 호버 배경색도 기본 배경색과 동일하게 설정
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, Red);
    if (ImGui::Button("DeleteLog"))
    {
       //
    }
    ImGui::PopStyleColor(3);

    //ImGui::Checkbox(); 검색 옵션

    //mGui::InputText(); 필터

    ImGui::Separator();
  
}