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

#include "CLevelSaveLoad.h"
#include <Engine\CLevelMgr.h>

void RTViewPort::render_update()
{
	CRenderMgr::GetInst()->CopyRTTex(m_ViewPortTexture);

	ImGui::Dummy(ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - 40));

	// 레벨 파일 드랍 체크
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			DWORD_PTR data = *((DWORD_PTR*)payload->Data);
			if (data == 0) return;
			string& str = *(string*)data;
			auto extension = path(str).extension().string();
			if (extension == ".lv") {
				auto pLevel = CLevelSaveLoad::LoadLevel(str);
				wstring levelname = path(str).stem().wstring();
				pLevel->SetName(levelname);
				GamePlayStatic::ChangeLevel(pLevel, LEVEL_STATE::STOP);
			}
		}

		ImGui::EndDragDropTarget();
	}

    ImGui::GetWindowDrawList()->AddImage(
        m_ViewPortTexture->GetSRV().Get(),
        ImVec2(0, 0),
        ImGui::GetWindowPos() + ImGui::GetWindowSize(),
        ImVec2(0, 0), ImVec2(1, 1));

    CImGuiMgr::GetInst()->GetbViewportFocused() = ImGui::IsWindowFocused(ImGuiFocusedFlags_None);

	
}
