#include "pch.h"
#include "ParticleSystemUI.h"



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

	ImGui::Text("dd");
}