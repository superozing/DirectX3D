#include "pch.h"
#include "SkyBoxUI.h"

#include <Engine\CSkyBox.h>

SkyBoxUI::SkyBoxUI()
	: ComponentUI("SkyBox", "##SkyBox", COMPONENT_TYPE::SKYBOX)
{
	SetComponentTitle("SkyBox");
}

SkyBoxUI::~SkyBoxUI()
{
}

void SkyBoxUI::render_update()
{
	if (!TitleCollapse("SkyBox")) return;

	string strSkyBoxType = ToString(magic_enum::enum_name((SKYBOX_TYPE)GetTargetObject()->SkyBox()->GetSkyBoxType()));
	Ptr<CTexture> pSphereTex = GetTargetObject()->SkyBox()->GetSphereTex();

	ImVec2 SkyBoxUISize = ImGui::GetContentRegionAvail();

	ImGui::Text("SkyBoxType");
	ImGui::SameLine();
	ImGui::InputText("##SkyBoxType", (char*)strSkyBoxType.c_str(), strSkyBoxType.length(), ImGuiInputTextFlags_ReadOnly);

	ImGui::Separator();

	ImGui::Text("Sphere Texture");
	
	static bool use_text_color_for_tint = false;
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	ImVec2 vScreenPos = ImGui::GetCursorScreenPos();
	ImGui::Image(pSphereTex->GetSRV().Get(), ImVec2(SkyBoxUISize.x, 150), uv_min, uv_max, tint_col, border_col);
	
}
