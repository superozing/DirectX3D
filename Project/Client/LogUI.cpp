#include "pch.h"
#include "LogUI.h"



LogUI::LogUI()
	: UI("LogUI", "##LogUI")
{
}

LogUI::~LogUI()
{
}

void LogUI::tick()
{
}

void LogUI::render_update()
{
    static float w = 200.0f;
    static float h = 300.0f;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::BeginChild("child1", ImVec2(w, h), true);
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::InvisibleButton("vsplitter", ImVec2(8.0f, h));
    if (ImGui::IsItemActive())
        w += ImGui::GetIO().MouseDelta.x;
    ImGui::SameLine();
    ImGui::BeginChild("child2", ImVec2(0, h), true);
    ImGui::EndChild();
    ImGui::InvisibleButton("hsplitter", ImVec2(-1, 8.0f));
    if (ImGui::IsItemActive())
        h += ImGui::GetIO().MouseDelta.y;
    ImGui::BeginChild("child3", ImVec2(0, 0), true);
    ImGui::EndChild();
    ImGui::PopStyleVar();

}