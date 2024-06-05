#include "pch.h"
#include "LandscapeUI.h"

#include <Engine\CLandScape.h>

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include <Engine\CRenderComponent.h>
#include <Engine\CMaterial.h>

#include <Engine\CHeightMapShader.h>



#define LandScapeTexturePathCount 18

LandScapeUI::LandScapeUI()
	: ComponentUI("LandScapeUI", "##LandScapeUI", COMPONENT_TYPE::LANDSCAPE)
	, m_vecHeightTextureKey()
	, m_vecBrushTextureKey()
	, m_vecLanderScapeMode()
	, m_matTessFactor{}
{
	GetLandScapeFileName();

	for (int i = 0; i < int(LANDSCAPE_MODE::NONE) + 1; ++i)
	{
		m_vecLanderScapeMode.push_back(ToString(magic_enum::enum_name((LANDSCAPE_MODE)i)));
	}
	
	int d = 0;
}

LandScapeUI::~LandScapeUI()
{
}

void LandScapeUI::render_update()
{
	if (!TitleCollapse("LandScape")) return;

	// 사전 준비
	static string strHeightMap = ToString(GetTargetObject()->LandScape()->GetHeightMapTex().Get()->GetKey());
	static string strBrush = ToString(GetTargetObject()->LandScape()->GetBrushTex().Get()->GetKey());
	static string strLandScapeMode = ToString(magic_enum::enum_name(GetTargetObject()->LandScape()->GetLandScapeMode()));

	m_matTessFactor = GetTargetObject()->LandScape()->GetEdgeTexFactor();

	static float fImageSize[2];
	fImageSize[0] = GetTargetObject()->LandScape()->GetHeightMapTex()->GetWidth();
	fImageSize[1] = GetTargetObject()->LandScape()->GetHeightMapTex()->GetHeight();

	static int iLandScapeFace[2];
	iLandScapeFace[0] = (int)(GetTargetObject()->LandScape()->GetLandScapeFaceX());
	iLandScapeFace[1] = (int)(GetTargetObject()->LandScape()->GetLandScapeFaceZ());

	static float fBrushSize[2];
	Vec2 vecBrushScale = GetTargetObject()->LandScape()->GetBrushScale();
	fBrushSize[0] = vecBrushScale.x;
	fBrushSize[1] = vecBrushScale.y;

	static float fBrushPow;
	fBrushPow = GetTargetObject()->LandScape()->GetBrushPow();

	static bool bTexup = true;

	 

	//동적 재질은 현재 play일때만 얻어올 수 있다.
	if (CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::PLAY)
		m_pTargetObjMtrl = GetTargetObject()->GetRenderComponent()->GetDynamicMaterial(0);
	else
		m_pTargetObjMtrl = GetTargetObject()->GetRenderComponent()->GetMaterial(0);

	static bool use_text_color_for_tint = false;
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	ImVec2 vScreenPos = ImGui::GetCursorScreenPos();

	// 높이 텍스쳐 정보
	StaticButton(string("LandScape Info"), STATIC_BTN_TYPE::SUBTITLE);

	ImGui::Image(GetTargetObject()->LandScape()->GetHeightMapTex()->GetSRV().Get(), ImVec2(ImGui::GetWindowSize().x, 150.f), uv_min, uv_max, tint_col, border_col);


	ImGui::Text("Height Map Texture "); ImGui::SameLine();

	if (ImGui::BeginCombo("##ComboHeightMap", strHeightMap.c_str()))
	{
		for (int n = 0; n < m_vecHeightTextureKey.size(); n++)
		{
			bool is_selected = (strHeightMap == m_vecHeightTextureKey[n]);
			if (ImGui::Selectable(m_vecHeightTextureKey[n].c_str(), is_selected))
			{
				strHeightMap = m_vecHeightTextureKey[n];

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

	ImGui::Text("LandScape Face"); ImGui::SameLine();
	if (ImGui::InputInt2("##LandScapeFace", iLandScapeFace))
	{
		GetTargetObject()->LandScape()->SetLandScapeFace((UINT)iLandScapeFace[0], true);
		GetTargetObject()->LandScape()->SetLandScapeFace((UINT)iLandScapeFace[1], false);

	}
	
	Vector4 TessDivide = GetTargetObject()->LandScape()->GetTessDivide();
	float fTessFactor[4] = { TessDivide.x, TessDivide.y, TessDivide.z, TessDivide.w };

	ImGui::Spacing();

	// land scape factor 설정
	StaticButton(string("LandScape Factor"), STATIC_BTN_TYPE::SUBTITLE);

	ImGui::Text("Tess Divide"); ImGui::SameLine();
	ImGui::DragFloat4("##Tess Factor", fTessFactor);

	GetTargetObject()->LandScape()->SetTessDivide(Vec4(fTessFactor[0] , fTessFactor[1], fTessFactor[2], fTessFactor[3]));

	ImGui::Spacing();

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;


	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Edge[0]"))
		{

			ImGui::Text("Min Level"); ImGui::SameLine();
			ImGui::InputFloat("##Edge[0]MinLevel", &m_matTessFactor._11, 1.f);

			ImGui::Text("Max Level"); ImGui::SameLine();
			ImGui::InputFloat("##Edge[0]MaxLevel", &m_matTessFactor._12, 1.f);

			ImGui::Text("Min Dist"); ImGui::SameLine();
			ImGui::InputFloat("##Edge[0]MinDist", &m_matTessFactor._13);

			ImGui::Text("Max Dist"); ImGui::SameLine();
			ImGui::InputFloat("##Edge[0]MaxDist", &m_matTessFactor._14);

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Edge[1]"))
		{
			ImGui::Text("Min Level"); ImGui::SameLine();
			ImGui::InputFloat("##Edge[1]MinLevel", &m_matTessFactor._21, 1.f);

			ImGui::Text("Max Level"); ImGui::SameLine();
			ImGui::InputFloat("##Edge[1]MaxLevel", &m_matTessFactor._22, 1.f);

			ImGui::Text("Min Dist"); ImGui::SameLine();
			ImGui::InputFloat("##Edge[1]MinDist", &m_matTessFactor._23);

			ImGui::Text("Max Dist"); ImGui::SameLine();
			ImGui::InputFloat("##Edge[1]MaxDist", &m_matTessFactor._24);
			
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Edge[2]"))
		{
			ImGui::Text("Min Level"); ImGui::SameLine();
			ImGui::InputFloat("##Edge[2]MinLevel", &m_matTessFactor._31, 1.f);

			ImGui::Text("Max Level"); ImGui::SameLine();
			ImGui::InputFloat("##Edge[2]MaxLevel", &m_matTessFactor._32, 1.f);

			ImGui::Text("Min Dist"); ImGui::SameLine();
			ImGui::InputFloat("##Edge[2]MinDist", &m_matTessFactor._33);

			ImGui::Text("Max Dist"); ImGui::SameLine();
			ImGui::InputFloat("##Edge[2]MaxDist", &m_matTessFactor._34);
			
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Inside"))
		{
			ImGui::Text("Min Level"); ImGui::SameLine();
			ImGui::InputFloat("##InsideMinLevel", &m_matTessFactor._41, 1.f);

			ImGui::Text("Max Level"); ImGui::SameLine();
			ImGui::InputFloat("##InsideMaxLevel", &m_matTessFactor._42, 1.f);

			ImGui::Text("Min Dist"); ImGui::SameLine();
			ImGui::InputFloat("##InsideMinDist", &m_matTessFactor._43);

			ImGui::Text("Max Dist"); ImGui::SameLine();
			ImGui::InputFloat("##InsideMaxDist", &m_matTessFactor._44);
			
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	GetTargetObject()->LandScape()->SetEdgeTexFactor(m_matTessFactor);

	ImGui::Spacing();

	// 랜드 스케이프 변형 ui

	StaticButton(string("LandScape Edit"), STATIC_BTN_TYPE::SUBTITLE);

	ImGui::Image(GetTargetObject()->LandScape()->GetBrushTex()->GetSRV().Get(), ImVec2(ImGui::GetWindowSize().x, 150.f), uv_min, uv_max, tint_col, border_col);

	ImGui::Text("LandScape Mode "); ImGui::SameLine();

	if (ImGui::BeginCombo("##ComboLandScapeMode", strLandScapeMode.c_str()))
	{
		for (int n = 0; n < m_vecLanderScapeMode.size(); n++)
		{
			bool is_selected = (strLandScapeMode == m_vecLanderScapeMode[n]);
			if (ImGui::Selectable(m_vecLanderScapeMode[n].c_str(), is_selected))
			{
				strLandScapeMode = m_vecLanderScapeMode[n];

				if (is_selected)
					ImGui::SetItemDefaultFocus();

				GetTargetObject()->LandScape()->SetLandScapeMode((LANDSCAPE_MODE)n);
			
			}
		}
		ImGui::EndCombo();
	}


	ImGui::Text("Brush Texture "); ImGui::SameLine();

	if (ImGui::BeginCombo("##ComboBrushTex", strBrush.c_str()))
	{
		for (int n = 0; n < m_vecBrushTextureKey.size(); n++)
		{
			bool is_selected = (strBrush == m_vecBrushTextureKey[n]);
			if (ImGui::Selectable(m_vecBrushTextureKey[n].c_str(), is_selected))
			{
				strBrush = m_vecBrushTextureKey[n];

				if (is_selected)
					ImGui::SetItemDefaultFocus();

				string m_strCurDirectory = "texture\\brush\\";
				wstring ParticleKey = ToWString(m_strCurDirectory += m_vecBrushTextureKey[n].c_str());

				GetTargetObject()->LandScape()->SetBrushTex(CAssetMgr::GetInst()->Load<CTexture>(ParticleKey, ParticleKey));

			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text("Brush Scale"); ImGui::SameLine();
	
	ImGui::DragFloat2("##Brush Scale", fBrushSize, 1.0f, 0.0f, FLT_MAX);
	vecBrushScale.x = fBrushSize[0];  vecBrushScale.y = fBrushSize[1];
	GetTargetObject()->LandScape()->SetBrushScale(vecBrushScale);

	ImGui::SameLine();

	ImGui::Text("Leveling Up"); ImGui::SameLine();
	if (ImGui::Checkbox("##TessUp", &bTexup))
	{
		GetTargetObject()->LandScape()->SetTessDir(bTexup);
	}

	ImGui::Text("Brush Power"); ImGui::SameLine();

	ImGui::DragFloat("##Brush Power", &fBrushPow, 1.0f, 0.0f, FLT_MAX);
	GetTargetObject()->LandScape()->SetBrushPow(fBrushPow);

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

	strFileName.clear();

	 m_strCurDirectory = "texture\\brush\\";
	 path = ToString(CPathMgr::GetContentPath()) + m_strCurDirectory;

	namespace fs = std::filesystem;
	for (const fs::directory_entry& entry : fs::directory_iterator(path)) {
		if (!entry.is_directory()) {
			auto filename = entry.path().filename().string();
			auto extension = entry.path().extension().string();

			strFileName.push_back(filename);
		}
	}

	this->m_vecBrushTextureKey = strFileName;

}



