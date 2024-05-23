#include "pch.h"
#include "LandscapeUI.h"

#include <Engine\CLandScape.h>

#define LandScapeTexturePathCount 18

LandScapeUI::LandScapeUI()
	: ComponentUI("LandScapeUI", "##LandScapeUI", COMPONENT_TYPE::LANDSCAPE)
	, m_vecHeightTextureKey()
{
	GetLandScapeFileName();
}

LandScapeUI::~LandScapeUI()
{
}

void LandScapeUI::render_update()
{
	if (!TitleCollapse("LandScape")) return;

	// 사전 준비
	static const char* charHeightMap = NULL;

	static float fImageSize[2];
	fImageSize[0] = GetTargetObject()->LandScape()->GetHeightMapTex()->GetWidth();
	fImageSize[1] = GetTargetObject()->LandScape()->GetHeightMapTex()->GetHeight();

	static int iLandScapeFace[2];
	iLandScapeFace[0] = (int)(GetTargetObject()->LandScape()->GetLandScapeFaceX());
	iLandScapeFace[1] = (int)(GetTargetObject()->LandScape()->GetLandScapeFaceZ());



	static bool use_text_color_for_tint = false;
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	ImVec2 vScreenPos = ImGui::GetCursorScreenPos();

	// 높이 텍스쳐 정보

	ImGui::Text("Height Map Texture");

	ImGui::Image(GetTargetObject()->LandScape()->GetHeightMapTex()->GetSRV().Get(), ImVec2(ImGui::GetWindowSize().x, 150.f), uv_min, uv_max, tint_col, border_col);


	ImGui::Text("Height Map Texture "); ImGui::SameLine();

	if (ImGui::BeginCombo("##ComboHeightMap", charHeightMap))
	{
		for (int n = 0; n < m_vecHeightTextureKey.size(); n++)
		{
			bool is_selected = (charHeightMap == m_vecHeightTextureKey[n].c_str());
			if (ImGui::Selectable(m_vecHeightTextureKey[n].c_str(), is_selected))
			{
				charHeightMap = m_vecHeightTextureKey[n].c_str();

				if (is_selected)
					ImGui::SetItemDefaultFocus();

				string m_strCurDirectory = "texture\\HeightMap\\";
				wstring ParticleKey = ToWString(m_strCurDirectory += m_vecHeightTextureKey[n].c_str());

				GetTargetObject()->LandScape()->SetHeightMapTex(CAssetMgr::GetInst()->Load<CTexture>(ParticleKey, ParticleKey));

			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text("Texture Size"); ImGui::SameLine();
	ImGui::InputFloat2("##Texture Size", fImageSize, nullptr, ImGuiInputTextFlags_ReadOnly);

	if (ImGui::InputInt2("LandScapeFace", iLandScapeFace))
	{
		GetTargetObject()->LandScape()->SetLandScapeFace((UINT)iLandScapeFace[0], true);
		GetTargetObject()->LandScape()->SetLandScapeFace((UINT)iLandScapeFace[1], false);

	}



}

void LandScapeUI::ResetUIinfo()
{
	GetLandScapeFileName();
}

void LandScapeUI::GetLandScapeFileName()
{
	vector<string> strFileName;

	string m_strCurDirectory = "texture\\HeightMap\\";
	string path = ToString(CPathMgr::GetContentPath()) + m_strCurDirectory;

	namespace fs = std::filesystem;
	for (const fs::directory_entry& entry : fs::directory_iterator(path)) {
		if (!entry.is_directory()) {
			auto filename = entry.path().filename().string();
			auto extension = entry.path().extension().string();

			strFileName.push_back(filename);
		}
	}

	this->m_vecHeightTextureKey = strFileName;
}