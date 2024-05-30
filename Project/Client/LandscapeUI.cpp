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
	static const char* charHeightMap = NULL;
	static const char* charBrush = NULL;
	static const char* charLandScapeMode = NULL;

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
		m_pTargetObjMtrl = GetTargetObject()->GetRenderComponent()->GetDynamicMaterial();
	else
		m_pTargetObjMtrl = GetTargetObject()->GetRenderComponent()->GetMaterial();

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

	ImGui::Text("LandScape Face"); ImGui::SameLine();
	if (ImGui::InputInt2("##LandScapeFace", iLandScapeFace))
	{
		GetTargetObject()->LandScape()->SetLandScapeFace((UINT)iLandScapeFace[0], true);
		GetTargetObject()->LandScape()->SetLandScapeFace((UINT)iLandScapeFace[1], false);

	}
	
	static Vector4* TessFactor = (Vector4*)(m_pTargetObjMtrl->GetScalarParam(SCALAR_PARAM::VEC4_0));
	float fTessFactor[4] = { (float)TessFactor->x, (float)TessFactor->y, (float)TessFactor->z, (float)TessFactor->w };

	ImGui::Text("Tess Factor"); ImGui::SameLine();
	ImGui::DragFloat4("##Tess Factor", fTessFactor);

	SetChangeTessFactor(TessFactor, fTessFactor);

	ImGui::Spacing();

	// 랜드 스케이프 변형 ui

	StaticButton(string("LandScape Edit"), STATIC_BTN_TYPE::SUBTITLE);

	ImGui::Image(GetTargetObject()->LandScape()->GetBrushTex()->GetSRV().Get(), ImVec2(ImGui::GetWindowSize().x, 150.f), uv_min, uv_max, tint_col, border_col);

	ImGui::Text("LandScape Mode "); ImGui::SameLine();

	if (ImGui::BeginCombo("##ComboLandScapeMode", charLandScapeMode))
	{
		for (int n = 0; n < m_vecLanderScapeMode.size(); n++)
		{
			bool is_selected = (charLandScapeMode == m_vecLanderScapeMode[n].c_str());
			if (ImGui::Selectable(m_vecLanderScapeMode[n].c_str(), is_selected))
			{
				charLandScapeMode = m_vecLanderScapeMode[n].c_str();

				if (is_selected)
					ImGui::SetItemDefaultFocus();

				GetTargetObject()->LandScape()->SetLandScapeMode((LANDSCAPE_MODE)n);
			
			}
		}
		ImGui::EndCombo();
	}


	ImGui::Text("Brush Texture "); ImGui::SameLine();

	if (ImGui::BeginCombo("##ComboBrushTex", charBrush))
	{
		for (int n = 0; n < m_vecBrushTextureKey.size(); n++)
		{
			bool is_selected = (charBrush == m_vecBrushTextureKey[n].c_str());
			if (ImGui::Selectable(m_vecBrushTextureKey[n].c_str(), is_selected))
			{
				charBrush = m_vecBrushTextureKey[n].c_str();

				if (is_selected)
					ImGui::SetItemDefaultFocus();

				string m_strCurDirectory = "texture\\brush\\";
				wstring ParticleKey = ToWString(m_strCurDirectory += m_vecBrushTextureKey[n].c_str());

				GetTargetObject()->LandScape()->SetBrushTex(CAssetMgr::GetInst()->Load<CTexture>(ParticleKey, ParticleKey));

			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text("Brush Scale");
	
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

void LandScapeUI::SetChangeTessFactor(Vec4* _mtrlparam, float* changevalue)
{
	_mtrlparam->x = changevalue[0];
	_mtrlparam->y = changevalue[1];
	_mtrlparam->z = changevalue[2];
	_mtrlparam->w = changevalue[3];

}


