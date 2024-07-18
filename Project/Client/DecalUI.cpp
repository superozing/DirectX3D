#include "pch.h"
#include "DecalUI.h"

#include <Engine\CDecal.h>
#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>

DecalUI::DecalUI()
	: ComponentUI("Decal", "##Decal", COMPONENT_TYPE::DECAL)
	, strDecalMtrl(" ")
	, strEmissiveCondition(" ")
	, iDecalPriority(0)
	, fDecalRenderDistance(0)
	, strCustomAlpha(" ")
	, fCustomAlpha(1.f)
{
	SetComponentTitle("Decal");
	strEmissiveCondition = "Emissive Off";
	strCustomAlpha		 = "Alpha Off";
}

DecalUI::~DecalUI()
{
}

#include <Engine\CLogMgr.h>
void DecalUI::render_update()
{
	ComponentUI::render_update();

	if (!TitleCollapse("Decal"))
		return;

	ImGui::Text("Decal Mtrl : ");
	ImGui::SameLine();

	strDecalMtrl = ToString(GetTargetObject()->GetRenderComponent()->GetMaterial(0)->GetKey());
	ImGui::InputText("##Decal Material", (char*)strDecalMtrl.c_str(), ImGuiInputTextFlags_ReadOnly);

	CMaterial* PayloadMaterial = nullptr;
	if (PayloadCheck(&PayloadMaterial))
	{
		GetTargetObject()->Decal()->ChangeMtrl(PayloadMaterial->GetKey());
	}

	ImGui::SeparatorText("Decal Texture");

	Ptr<CTexture> pTex = GetTargetObject()->GetRenderComponent()->GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_0).Get();

	if (pTex.Get() != nullptr)
	{
		ImVec2 UIsize = ImGui::GetWindowSize();

		static bool use_text_color_for_tint = false;
		ImVec2		uv_min					= ImVec2(0.0f, 0.0f); // Top-left
		ImVec2		uv_max					= ImVec2(1.0f, 1.0f); // Lower-right
		ImVec4		tint_col				= use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text)
																	  : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
		ImVec4		border_col				= ImGui::GetStyleColorVec4(ImGuiCol_Border);

		ImGui::Image(pTex->GetSRV().Get(), ImVec2(UIsize.x - 10.f, 150.f), uv_min, uv_max, tint_col, border_col);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		pTex = GetTargetObject()->GetRenderComponent()->GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_1).Get();

		if (pTex.Get() != nullptr)
			ImGui::Image(pTex->GetSRV().Get(), ImVec2(UIsize.x - 10.f, 150.f), uv_min, uv_max, tint_col, border_col);
		else
			ImGui::Text("No Decal Emissive Texture");
	}
	else
		ImGui::Text("No Decal Texture");

	ImGui::SeparatorText("Decal Option");

	ImGui::Text("Decal Emissive : ");
	ImGui::SameLine();

	// Emissive 설정
	if (ImGui::Button(strEmissiveCondition.c_str(), ImVec2(120.f, 20.f)))
	{
		int iEmissive = GetTargetObject()->Decal()->GetDecalAsEmissive();

		if (iEmissive > 0)
		{
			strEmissiveCondition = "Emissive Off";
			GetTargetObject()->Decal()->SetDecalAsEmissive(0);
		}
		else
		{
			strEmissiveCondition = "Emissive On";
			GetTargetObject()->Decal()->SetDecalAsEmissive(1);
		}
	}

	ImGui::Text("Decal Priority");
	ImGui::SameLine();

	iDecalPriority = GetTargetObject()->Decal()->GetDecalPriority();
	if (ImGui::InputInt("##Decal Priority", &iDecalPriority))
	{
		if (iDecalPriority > DecalPriorityMax)
			iDecalPriority = DecalPriorityMax;

		if (iDecalPriority < 0)
			iDecalPriority = 0;

		GetTargetObject()->Decal()->SetDecalPriority(iDecalPriority);
	}

	ImGui::Text("Decal Render Distance");
	ImGui::SameLine();

	fDecalRenderDistance = GetTargetObject()->Decal()->GetRenderDistance();
	if (ImGui::DragFloat("##DecalRenderDistance", &fDecalRenderDistance))
	{
		GetTargetObject()->Decal()->SetRenderDistance(fDecalRenderDistance);
	}

	ImGui::Text("Decal Custom Alpha : ");
	ImGui::SameLine();

	// CustomAlpha 설정
	if (ImGui::Button(strCustomAlpha.c_str(), ImVec2(120.f, 20.f)))
	{
		int iCAlpha = GetTargetObject()->Decal()->GetUsetCustomAlpha();

		if (iCAlpha > 0)
		{
			strCustomAlpha = "Alpha Off";
			GetTargetObject()->Decal()->SetUseCustomAlpha(0);
		}
		else
		{
			strCustomAlpha = "Alpha On";
			GetTargetObject()->Decal()->SetUseCustomAlpha(1);
		}
	}

	ImGui::Text("Decal Custom Alpha");
	ImGui::SameLine();

	fCustomAlpha = GetTargetObject()->Decal()->GetCustomAlpha();
	if (ImGui::DragFloat("##DecalCustomfAlpha", &fCustomAlpha, 0.01f, 0.f, 1.f))
	{
		GetTargetObject()->Decal()->SetCustomAlpha(fCustomAlpha);
	}
}