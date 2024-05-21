#include "pch.h"
#include "ParticleSystemUI.h"

#include <Engine\CParticleSystem.h>

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("ParticleSystemUI", "##ParticleSystemUI", COMPONENT_TYPE::PARTICLESYSTEM)
{
	SetComponentTitle("ParticleSystem");
}

ParticleSystemUI::~ParticleSystemUI()
{
}

void ParticleSystemUI::render_update()
{
	if (!TitleCollapse("ParticleSystem")) return;

	// 사전 세팅

	m_TargetParticleComponent = dynamic_cast<CParticleSystem*>(GetTargetObject()->GetRenderComponent());
	m_ParticleTexture = m_TargetParticleComponent->GetParticleTex().Get();

	ImVec2 UIsize = ImGui::GetWindowSize();


	static bool use_text_color_for_tint = false;
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);

	// 파티클 텍스쳐
	StaticButton(string("Particle Texture"), STATIC_BTN_TYPE::SUBTITLE);
	ImGui::Image(m_ParticleTexture->GetSRV().Get(), ImVec2(UIsize.x - 10.f, 150.f), uv_min, uv_max, tint_col, border_col);

	ImGui::Spacing();

	StaticButton(string("Particle Detail"), STATIC_BTN_TYPE::SUBTITLE);

	ImGui::Text("Max Count"); ImGui::SameLine();
	string strMaxParticle = std::to_string(m_TargetParticleComponent->GetMaxParticleCount());
	ImGui::SetNextItemWidth(80);
	ImGui::InputText("##Max Count", (char*)strMaxParticle.c_str(), strMaxParticle.length(), ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Current Count"); ImGui::SameLine();
	//string strMaxParticle = std::to_string(m_TargetParticleComponent->GetMaxParticleCount());
}