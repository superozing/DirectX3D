#include "pch.h"
#include "DecalUI.h"

#include <Engine\CDecal.h>
#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>

DecalUI::DecalUI()
	: ComponentUI("Decal", "##Decal", COMPONENT_TYPE::DECAL)
	, strEmissiveCondition(" ")
{
	SetComponentTitle("Decal");
	strEmissiveCondition = "Off";
}

DecalUI::~DecalUI()
{
}

void DecalUI::render_update()
{
	ComponentUI::render_update();

	if (!TitleCollapse("Decal"))
		return;

	ImGui::SeparatorText("Decal Texture");

	ImVec2 UIsize = ImGui::GetWindowSize();

	static bool use_text_color_for_tint = false;
	ImVec2		uv_min					= ImVec2(0.0f, 0.0f); // Top-left
	ImVec2		uv_max					= ImVec2(1.0f, 1.0f); // Lower-right
	ImVec4		tint_col =
		 use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);

	ImGui::Image(GetTargetObject()->GetRenderComponent()->GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_0).Get(),
				 ImVec2(UIsize.x - 10.f, 150.f), uv_min, uv_max, tint_col, border_col);

	ImGui::SeparatorText("Decal Option");

	ImGui::Text("Decal Emissive : ");
	ImGui::SameLine();

	// Emissive 설정
	if (ImGui::Button(strEmissiveCondition.c_str(), ImVec2(40.f, 20.f)))
	{
		int iEmissive = GetTargetObject()->Decal()->GetDecalAsEmissive();

		if (iEmissive > 0)
		{
			strEmissiveCondition = "Off";
			GetTargetObject()->Decal()->SetDecalAsEmissive(0);
		}
		else
		{
			strEmissiveCondition = "On";
			GetTargetObject()->Decal()->SetDecalAsEmissive(1);
		}
	}
}