#include "pch.h"
#include "LandscapeUI.h"

#include <Engine\CLandScape.h>

#define LandScapeTexturePathCount 18

LandScapeUI::LandScapeUI()
	: ComponentUI("LandScapeUI", "##LandScapeUI", COMPONENT_TYPE::LANDSCAPE)
{
}

LandScapeUI::~LandScapeUI()
{
}

void LandScapeUI::render_update()
{
	if (!TitleCollapse("LandScape")) return;

	static bool use_text_color_for_tint = false;
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	ImVec2 vScreenPos = ImGui::GetCursorScreenPos();

	// 높이 텍스쳐 정보

	ImGui::Text("Height Map Texture");

	ImGui::Image(GetTargetObject()->LandScape()->GetHeightMapTex()->GetSRV().Get(), ImVec2(ImGui::GetWindowSize().x, 150.f), uv_min, uv_max, tint_col, border_col);

	float fImageSize[2];
	fImageSize[0] = GetTargetObject()->LandScape()->GetHeightMapTex()->GetWidth();
	fImageSize[1] = GetTargetObject()->LandScape()->GetHeightMapTex()->GetHeight();

	ImGui::Text("Texture Name"); ImGui::SameLine();
	string strHeightTexName = ToString(GetTargetObject()->LandScape()->GetHeightMapTex()->GetKey());
	strHeightTexName = strHeightTexName.substr(LandScapeTexturePathCount, strHeightTexName.size());
	ImGui::InputText("##Height Map TextureName", (char*)strHeightTexName.c_str(), ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Texture Size"); ImGui::SameLine();
	ImGui::InputFloat2("##Texture Size", fImageSize, nullptr, ImGuiInputTextFlags_ReadOnly);
}