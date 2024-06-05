#include "pch.h"
#include "ParticleSystemUI.h"

#include <Engine\CParticleSystem.h>

#define ParticleFolderPathCount 17

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("ParticleSystemUI", "##ParticleSystemUI", COMPONENT_TYPE::PARTICLESYSTEM)
	, m_vecParticleKey()
	, m_iNewMaxCount(0)
{
	SetComponentTitle("ParticleSystem");
	GetParticleFileName();

	m_ModuleButtonColor[0] = { 0.5f, 0.5f, 0.5f, 1.0f }; // normal
	m_ModuleButtonColor[1] = { 1.0f, 1.0f, 0.0f, 1.0f }; // hover

}

ParticleSystemUI::~ParticleSystemUI()
{
}

void ParticleSystemUI::render_update()
{
	if (!TitleCollapse("ParticleSystem")) return;

	// 사전 세팅
	CGameObject* Targetobj = GetTargetObject();
	CParticleSystem* pPS = Targetobj->ParticleSystem();
	tParticleModule CurModule = pPS->GetParticleModule();
	tParticleModule NewModule = CurModule;

	static string strParticleTex = ToString(GetTargetObject()->ParticleSystem()->GetParticleTex().Get()->GetKey());



	ImVec2 UIsize = ImGui::GetWindowSize();

	static bool use_text_color_for_tint = false;
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);

	// 파티클 텍스쳐
	StaticButton(string("Particle Texture"), STATIC_BTN_TYPE::SUBTITLE);

	//Particle ComboBox
	ImGui::Text("Texture");
	ImGui::SameLine();

	if (ImGui::BeginCombo("##ComboParticle", strParticleTex.c_str()))
	{
		for (int n = 0; n < m_vecParticleKey.size(); n++)
		{
			bool is_selected = (strParticleTex == m_vecParticleKey[n]);
			if (ImGui::Selectable(m_vecParticleKey[n].c_str(), is_selected))
			{
				strParticleTex = m_vecParticleKey[n];

				if (is_selected)
					ImGui::SetItemDefaultFocus();

				string m_strCurDirectory = "texture\\particle\\";
				wstring ParticleKey = ToWString(m_strCurDirectory += m_vecParticleKey[n].c_str());

				GetTargetObject()->ParticleSystem()->SetParticleTex(CAssetMgr::GetInst()->Load<CTexture>(ParticleKey, ParticleKey));

			}
		}
		ImGui::EndCombo();
	}

	ImGui::Image(GetTargetObject()->ParticleSystem()->GetParticleTex()->GetSRV().Get(), ImVec2(UIsize.x - 10.f, 150.f), uv_min, uv_max, tint_col, border_col);

	string strTextureName = ToString(GetTargetObject()->ParticleSystem()->GetParticleTex()->GetKey());
	strTextureName = strTextureName.substr(ParticleFolderPathCount, strTextureName.size());
	
	ImGui::Text("Particle Texture : "); ImGui::SameLine();
	ImGui::InputText("##ParticleTexName", (char*)strTextureName.c_str(), strTextureName.length(), ImGuiInputTextFlags_ReadOnly);
	
	ImGui::Spacing();


	ImGui::Text("Max Count"); ImGui::SameLine();
	
	int iMaxParticle;
	iMaxParticle = UINT(GetTargetObject()->ParticleSystem()->GetMaxParticleCount());
	
	ImGui::SetNextItemWidth(100);
	
	if (ImGui::InputInt("##Max Count", &iMaxParticle))
	{
		m_iNewMaxCount = iMaxParticle;

	}

	ImGui::SameLine();

	if (ImGui::Button("Apply", ImVec2(50.f, 20.f)))
	{
		if (m_iNewMaxCount < 0)
		{
			m_iNewMaxCount = 0;
		}

		GetTargetObject()->ParticleSystem()->SetMaxParticleCount((UINT)m_iNewMaxCount);
	}
	
	ImGui::Separator();

	// Spawn Module

	StaticButton(string("Spawn Module"), STATIC_BTN_TYPE::SUBTITLE);

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, m_ModuleButtonColor[0]);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_ModuleButtonColor[1]);
	if (1 == NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN]) {
		
		if (ImGui::Button("On##SpawnModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 0;
		}
		
	}
	else {

		if (ImGui::Button("Off##SpawnModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;
		}

	}
	ImGui::PopStyleColor(2);

	if(NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] == 1)
	{ 
		ImGui::Text("SpaceType"); ImGui::SameLine();
		if (1 == NewModule.SpaceType) {
			if (ImGui::Button("WorldSpace")) {
				NewModule.SpaceType = 0;
			}
		}
		else {
			if (ImGui::Button("LocalSpace")) {
				NewModule.SpaceType = 1;
			}
		}
		ImGui::Text("Spawn Color"); ImGui::SameLine();
		ImGui::ColorEdit4("##Spawn Color", &NewModule.vSpawnColor.x);
		ImGui::Text("Min Scale"); ImGui::SameLine();
		ImGui::DragFloat4("##Min Scale", &NewModule.vSpawnMinScale.x, 0.1f, 0.0f, 1000.0f);
		ImGui::Text("Max Scale"); ImGui::SameLine();
		ImGui::DragFloat4("##Max Scale", &NewModule.vSpawnMaxScale.x, 0.1f, 0.0f, 1000.0f);
		ImGui::Text("Min Life"); ImGui::SameLine();
		ImGui::DragFloat("##Min Life", &NewModule.MinLife, 0.1f, 0.0f, 100.0f);
		ImGui::Text("Max Life"); ImGui::SameLine();
		ImGui::DragFloat("##Max Life", &NewModule.MaxLife, 0.1f, 0.0f, 100.0f);
		ImGui::Text("Min Mass"); ImGui::SameLine();
		ImGui::DragFloat("##Min Mass", &NewModule.MinMass, 0.1f, 0.0f, 100.0f);
		ImGui::Text("Max Mass"); ImGui::SameLine();
		ImGui::DragFloat("##Max Mass", &NewModule.MaxMass, 0.1f, 0.0f, 100.0f);
		ImGui::Text("Spawn Shape"); ImGui::SameLine();
		if (1 == NewModule.SpawnShape) {
			if (ImGui::Button("Box")) {
				NewModule.SpawnShape = 0;
			}
		}
		else {
			if (ImGui::Button("Sphere")) {
				NewModule.SpawnShape = 1;
			}
		}
		ImGui::Text("Radius"); ImGui::SameLine();
		ImGui::DragFloat("##Radius", &NewModule.Radius, 1.0f, 0.0f, 1000.0f);
		ImGui::Text("Box Scale"); ImGui::SameLine();
		ImGui::DragFloat4("##Box Scale", &NewModule.vSpawnBoxScale.x, 0.1f, 0.0f, 100.0f);
		ImGui::Text("Spawn Rate"); ImGui::SameLine();
		ImGui::DragInt("##Spawn Rate", &NewModule.SpawnRate, 1, 0, 1000);
	}
		ImGui::Spacing();

	// Velocity Module

	StaticButton(string("Velocity Module"), STATIC_BTN_TYPE::SUBTITLE);

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, m_ModuleButtonColor[0]);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_ModuleButtonColor[1]);

	if (1 == NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY]) {
		
		if (ImGui::Button("On##AddVelocityModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 0;
		}
	
	}
	else {
		
		if (ImGui::Button("Off##AddVelocityModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 1;
		}
		
	}
	
	ImGui::PopStyleColor(2);

	if(NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] == 1)
	{ 
	ImGui::Text("AddVelocityType"); ImGui::SameLine();
	
	if (0 == NewModule.AddVelocityType) {
		if (ImGui::Button("From Center")) {
			NewModule.AddVelocityType = 1;
		}
	}
	else if (1 == NewModule.AddVelocityType) {
		if (ImGui::Button("To Center")) {
			NewModule.AddVelocityType = 2;
		}
	}
	else
	{
		if (ImGui::Button("Fix Direction")) {
			NewModule.AddVelocityType = 0;
		}
	}
		ImGui::Text("Min Speed"); ImGui::SameLine();
		ImGui::DragFloat("##Min Speed", &NewModule.MinSpeed, 1, 0, 1000);
		ImGui::Text("Max Speed"); ImGui::SameLine();
		ImGui::DragFloat("##Max Speed", &NewModule.MaxSpeed, 1, 0, 1000);
		ImGui::Text("FixedDirection"); ImGui::SameLine();
		ImGui::DragFloat4("##FixedDirection", &NewModule.FixedDirection.x, 0.1, 0, 1000);
		ImGui::Text("FixedAngle"); ImGui::SameLine();
		ImGui::DragFloat("##FixedAngle", &NewModule.FixedAngle, 0.1, 0, 1000);
	}

	ImGui::Spacing();

	//Drag Module

	StaticButton(string("Drag Module"), STATIC_BTN_TYPE::SUBTITLE);

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, m_ModuleButtonColor[0]);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_ModuleButtonColor[1]);
	if (1 == NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG])
	{

		if (ImGui::Button("On##DragModule"))
		{
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = 0;
		}
	
	}
	else
	{
		
		if (ImGui::Button("Off##DragModule"))
		{
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = 1;
		}
		
	}
	ImGui::PopStyleColor(2);
	
	if(NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] == 1)
	{ 
		ImGui::Text("DragTime"); ImGui::SameLine();
		ImGui::DragFloat("##DragTime", &NewModule.DragTime, 0.01f, 0.01f, 10000.f);
	}
	
	ImGui::Spacing();

	// Scale Module

	StaticButton(string("Scale Module"), STATIC_BTN_TYPE::SUBTITLE);

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, m_ModuleButtonColor[0]);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_ModuleButtonColor[1]);
	if (1 == NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE]) {
		if (ImGui::Button("On##ScaleModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = 0;
		}

	}
	else {
		
		if (ImGui::Button("Off##ScaleModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = 1;
		}

	}
	ImGui::PopStyleColor(2);

	if(NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] == 1)
	{ 
		ImGui::Text("Scale Ratio"); ImGui::SameLine();
		ImGui::DragFloat3("##Scale Ratio", &NewModule.vScaleRatio.x, 0.01f, 0.0f, 10.0f);
	}
	
	ImGui::Spacing();

	// Noise Force
	StaticButton(string("Noise Force Module"), STATIC_BTN_TYPE::SUBTITLE);

	ImGui::SameLine();
	
	ImGui::PushStyleColor(ImGuiCol_Button, m_ModuleButtonColor[0]);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_ModuleButtonColor[1]);
	if (1 == NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE]) {
		
		if (ImGui::Button("On##NoiseForce")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 0;
		}
		
	}
	else {
		
		if (ImGui::Button("Off##NoiseForce")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 1;
		}
	}
	ImGui::PopStyleColor(2);

	if(NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] == 1)
	{ 
		ImGui::Text("NoiseForceScale"); ImGui::SameLine();
		ImGui::DragFloat("##NoiseForceScale", &NewModule.NoiseForceScale, 0.1f, 0.0f, 100.0f);
		ImGui::Text("NoiseForceTerm"); ImGui::SameLine();
		ImGui::DragFloat("##NoiseForceTerm", &NewModule.NoiseForceTerm, 0.01f, 0.0f, 1.0f);
	}

	ImGui::Spacing();
	
	// Calculate Force
	StaticButton(string("Calculate Module"), STATIC_BTN_TYPE::SUBTITLE);

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, m_ModuleButtonColor[0]);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_ModuleButtonColor[1]);
	ImGui::SameLine();
	if (1 == NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE]) {

		if (ImGui::Button("On##CalculateForce")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = 0;
		}
	
	}
	else {
		
		if (ImGui::Button("Off##CalculateForce")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = 1;
		}
	
	}
	ImGui::PopStyleColor(2);

	ImGui::Spacing();

	// Render Module
	
	StaticButton(string("Render Module"), STATIC_BTN_TYPE::SUBTITLE);

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, m_ModuleButtonColor[0]);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_ModuleButtonColor[1]);
	if (1 == NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER]) {
		
		if (ImGui::Button("On##Render")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 0;
		}
		
	}
	else {
		
		if (ImGui::Button("Off##Render")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 1;
		}
		
	}
	ImGui::PopStyleColor(2);

	if(NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] == 1)
	{ 
		ImGui::Text("VelocityAlignment"); ImGui::SameLine();
		if (0 == NewModule.VelocityAlignment) {
			if (ImGui::Button("Off")) {
				NewModule.VelocityAlignment = 1;
			}
		}
		else if (1 == NewModule.VelocityAlignment) {
			if (ImGui::Button("On")) {
				NewModule.VelocityAlignment = 0;
			}
		}
		ImGui::Text("AlphaBasedLife"); ImGui::SameLine();
		if (0 == NewModule.AlphaBasedLife) {
			if (ImGui::Button("Off")) {
				NewModule.AlphaBasedLife = 1;
			}
		}
		else if (1 == NewModule.AlphaBasedLife) {
			if (ImGui::Button("NomrlizedAge")) {
				NewModule.AlphaBasedLife = 2;
			}
		}
		else
		{
			if (ImGui::Button("Age")) {
				NewModule.AlphaBasedLife = 0;
			}
		}
		ImGui::Text("AlphaMaxAge"); ImGui::SameLine();
		ImGui::DragFloat("##AlphaMaxAge", &NewModule.AlphaMaxAge, 0.1f, 0.0f, 10.0f);

	}
	pPS->SetModule(NewModule);

}

void ParticleSystemUI::ResetUIinfo()
{
	GetParticleFileName();
}

void ParticleSystemUI::GetParticleFileName()
{
	vector<string> strFileName;

	string m_strCurDirectory = "texture\\particle\\";
	string path = ToString(CPathMgr::GetContentPath()) + m_strCurDirectory;

	namespace fs = std::filesystem;
	for (const fs::directory_entry& entry : fs::directory_iterator(path)) {
		if (!entry.is_directory()) {
			auto filename = entry.path().filename().string();
			auto extension = entry.path().extension().string();

			strFileName.push_back(filename);
		}
	}

	this->m_vecParticleKey = strFileName;
}
