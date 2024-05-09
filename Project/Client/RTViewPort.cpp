#include "pch.h"
#include "RTViewPort.h"

#include <Engine\CRenderMgr.h>
#include <Engine\CDevice.h>
#include "CImGuiMgr.h"

RTViewPort::RTViewPort()
	: UI("Viewport", "##Viewport")
{
	
	m_ViewPortTexture = CAssetMgr::GetInst()->CreateTexture(L"CopyRTtex",
			CDevice::GetInst()->GetRenderResolution().x,
			CDevice::GetInst()->GetRenderResolution().y,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_SHADER_RESOURCE);
	
}

RTViewPort::~RTViewPort()
{
}

void RTViewPort::tick()
{
}

void RTViewPort::render_update()
{
	CRenderMgr::GetInst()->CopyRTTex(m_ViewPortTexture);

    ImGui::GetWindowDrawList()->AddImage(
        m_ViewPortTexture->GetSRV().Get(),
        ImGui::GetCursorScreenPos(),
        ImGui::GetWindowPos() + ImGui::GetWindowSize(),
        ImVec2(0, 0), ImVec2(1, 1));

    CImGuiMgr::GetInst()->GetbViewportFocused() = ImGui::IsWindowFocused(ImGuiFocusedFlags_None);
}
