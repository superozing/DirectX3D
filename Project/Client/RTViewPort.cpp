#include "pch.h"
#include "RTViewPort.h"

#include "CImGuiMgr.h"

RTViewPort::RTViewPort()
	: UI("Viewport", "##Viewport")
{
}

RTViewPort::~RTViewPort()
{
}

void RTViewPort::tick()
{
}

void RTViewPort::render_update()
{
    ImGui::GetWindowDrawList()->AddImage(
        CImGuiMgr::GetInst()->GetViewportTexture()->GetSRV().Get(),
        ImGui::GetCursorScreenPos(),
        ImGui::GetWindowPos() + ImGui::GetWindowSize(),
        ImVec2(0, 0), ImVec2(1, 1));

    CImGuiMgr::GetInst()->GetbViewportFocused() = ImGui::IsWindowFocused(ImGuiFocusedFlags_None);
}
