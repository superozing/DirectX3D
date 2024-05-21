#include "pch.h"
#include "SkyBoxUI.h"

#include <Engine\CSkyBox.h>

SkyBoxUI::SkyBoxUI()
	: ComponentUI("SkyBox", "##SkyBox", COMPONENT_TYPE::SKYBOX)
{
	SetComponentTitle("SkyBox");

	m_vecSkyBoxKey = GetSkyBoxFile();
}

SkyBoxUI::~SkyBoxUI()
{
}

void SkyBoxUI::render_update()
{
	if (!TitleCollapse("SkyBox")) return;

	Ptr<CTexture> pSphereTex = GetTargetObject()->SkyBox()->GetSphereTex();
	Ptr<CTexture> pCubeTex = GetTargetObject()->SkyBox()->GetCubeTex();

	ImVec2 SkyBoxUISize = ImGui::GetContentRegionAvail();

	static const char* cSelectSphere = NULL;
	static const char* cSelectCube = NULL;


	ImGui::Text("SkyBoxType");
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(1.f, 1.f, 1.f, 1.f));
	
	static int iSkyBoxType = (UINT)(GetTargetObject()->SkyBox()->GetSkyBoxType());
	
	if (ImGui::RadioButton("Sphere Type", &iSkyBoxType, 0))
	{
		GetTargetObject()->SkyBox()->SetSkyBoxType(SKYBOX_TYPE::SPHERE);
	}
	ImGui::SameLine();

	if (ImGui::RadioButton("Cube Type", &iSkyBoxType, 1))
	{
		GetTargetObject()->SkyBox()->SetSkyBoxType(SKYBOX_TYPE::CUBE);
	}
	ImGui::SameLine();

	ImGui::PopStyleColor();
	ImGui::Spacing(); 

	ImGui::SeparatorText("Sphere Texture");

	static bool use_text_color_for_tint = false;
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	ImVec2 vScreenPos = ImGui::GetCursorScreenPos();
	

	if(pSphereTex != nullptr)
	{ 
		ImGui::Image(pSphereTex->GetSRV().Get(), ImVec2(SkyBoxUISize.x, 150), uv_min, uv_max, tint_col, border_col);
		
		m_strTextureName[0] = ToString(pSphereTex.Get()->GetKey());
	
		ImGui::Text("Texture Key  ");
		ImGui::SameLine();
		m_strTextureName[0] = ToString(pSphereTex.Get()->GetKey());
		ImGui::InputText("##Texture Key Sphere", (char*)m_strTextureName->c_str(), m_strTextureName->length(), ImGuiInputTextFlags_ReadOnly);

		// 해상도 정보 출력
		float Width = pSphereTex->GetWidth();
		float Height = pSphereTex->GetHeight();

		ImGui::Text("Width        ");
		ImGui::SameLine();
		ImGui::InputFloat("##SphereTextureWidth", &Width, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);

		ImGui::Text("Height       ");
		ImGui::SameLine();
		ImGui::InputFloat("##SphereTextureHeight", &Height, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);

	
	}
	else
	{
		m_strTextureName[0] = "";

		ImGui::Text("Texture Key  ");
		ImGui::SameLine();
		ImGui::InputText("##Texture Key Sphere", (char*)m_strTextureName->c_str(), m_strTextureName->length(), ImGuiInputTextFlags_ReadOnly);
	}

	ImGui::Text("Change Texture");
	ImGui::SameLine();

	if (ImGui::BeginCombo("##comboSkyBoxSphere", cSelectSphere)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < m_vecSkyBoxKey.size(); n++)
		{
			bool is_selected = (cSelectSphere == m_vecSkyBoxKey[n].c_str()); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(m_vecSkyBoxKey[n].c_str(), is_selected))
			{
				cSelectSphere = m_vecSkyBoxKey[n].c_str();
				
				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)

				string m_strCurDirectory = "texture\\skybox\\";
				wstring SkyBoxKey = ToWString(m_strCurDirectory += m_vecSkyBoxKey[n].c_str());

				GetTargetObject()->SkyBox()->SetSphereTexture(CAssetMgr::GetInst()->Load<CTexture>(SkyBoxKey, SkyBoxKey));

			}
		}
		ImGui::EndCombo();
	}


	ImGui::Spacing();

	ImGui::SeparatorText("Cube Texture");

	use_text_color_for_tint = false;
	uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
	border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	vScreenPos = ImGui::GetCursorScreenPos();
	
	if (pCubeTex != nullptr)
	{ 
		//ImGui::Image(pCubeTex->GetSRV().Get(), ImVec2(SkyBoxUISize.x, 150), uv_min, uv_max, tint_col, border_col);
		
		m_strTextureName[1] = ToString(pCubeTex.Get()->GetKey());

		ImGui::Text("Texture Key  ");
		ImGui::SameLine();
		ImGui::InputText("##Texture Key Cube", (char*)m_strTextureName->c_str(), m_strTextureName->length(), ImGuiInputTextFlags_ReadOnly);


		// 해상도 정보 출력
		float Width = pCubeTex->GetWidth();
		float Height = pCubeTex->GetHeight();

		ImGui::Text("Width        ");
		ImGui::SameLine();
		ImGui::InputFloat("##CubeTextureWidth", &Width, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);

		ImGui::Text("Height       ");
		ImGui::SameLine();
		ImGui::InputFloat("##CubeTextureHeight", &Height, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);


	}
	else
	{
		m_strTextureName[0] = "";
		
		ImGui::Text("Texture Key  ");
		ImGui::SameLine();
		ImGui::InputText("##Texture Key Cube", (char*)m_strTextureName->c_str(), m_strTextureName->length(), ImGuiInputTextFlags_ReadOnly);

	}

	ImGui::Text("Change Texture");
	ImGui::SameLine();

	if (ImGui::BeginCombo("##comboSkyBoxCube", cSelectCube)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < m_vecSkyBoxKey.size(); n++)
		{
			bool is_selected = (cSelectCube == m_vecSkyBoxKey[n].c_str()); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(m_vecSkyBoxKey[n].c_str(), is_selected))
			{
				cSelectCube = m_vecSkyBoxKey[n].c_str();

				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)

				string m_strCurDirectory = "texture\\skybox\\";
				wstring SkyBoxKey = ToWString(m_strCurDirectory += m_vecSkyBoxKey[n].c_str());

				GetTargetObject()->SkyBox()->SetCubeTexture(CAssetMgr::GetInst()->Load<CTexture>(SkyBoxKey, SkyBoxKey));

			}
		}
		ImGui::EndCombo();
	}

}

vector<string> SkyBoxUI::GetSkyBoxFile()
{
	vector<string> strFileName;

	string m_strCurDirectory = "texture\\skybox\\";
	string path = ToString(CPathMgr::GetContentPath()) + m_strCurDirectory;

	namespace fs = std::filesystem;
	for (const fs::directory_entry& entry : fs::directory_iterator(path)) {
		if (!entry.is_directory()) {
			auto filename = entry.path().filename().string();
			auto extension = entry.path().extension().string();

			strFileName.push_back(filename);
		}
	}

	return strFileName;
}
