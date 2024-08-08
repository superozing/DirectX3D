#include "pch.h"
#include "SettingUI.h"

#include "MenuUI.h"

#include <Engine\CDevice.h>
#include <Engine\CSound.h>
#include <Engine\CKeyMgr.h>

#include <Scripts\CPlayerController.h>
#include <Scripts\CButtons.h>

#include "ParamUI.h"
void SettingActive()
{
	CImGuiMgr::GetInst()->FindUI("##Setting")->Activate();
}

SettingUI::SettingUI()
	: UI("Setting", "##Setting")
{
	CButtons::Setting = &(SettingActive);
	AddFlag(ImGuiWindowFlags_MenuBar);
}

SettingUI::~SettingUI()
{
}

void SettingUI::tick()
{
}

void SettingUI::render_update()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::MenuItem("Display"))
		{
			select_render = &SettingUI::Display;
		}
		if (ImGui::MenuItem("Sound"))
		{
			select_render = &SettingUI::Sound;
		}
		if (ImGui::MenuItem("Graphic"))
		{
			select_render = &SettingUI::Graphic;
		}
		if (ImGui::MenuItem("Control"))
		{
			select_render = &SettingUI::Control;
		}

		ImGui::EndMenuBar();
	}

	if (select_render)
		(this->*select_render)();
}

void SettingUI::Activate()
{
	UI::Activate();

	select_render = &SettingUI::Graphic;
}

void SettingUI::Display()
{
	int ResolutionX = int(CDevice::GetInst()->GetRenderResolution().x);
	int ResolutionY = int(CDevice::GetInst()->GetRenderResolution().y);

	string Resolution = to_string(ResolutionX) + " X " + to_string(ResolutionY);

	static bool IsWindowMode = true;
	if (ImGui::Checkbox("Window Mode", &IsWindowMode))
	{
		GamePlayStatic::ScreenResize(CDevice::GetInst()->GetRenderResolution(), IsWindowMode);
	}

	if (ImGui::MenuItem("1366 X 720"))
	{
		GamePlayStatic::ScreenResize(Vec2(1366.f, 720.f), IsWindowMode);
	}

	if (ImGui::MenuItem("1910 X 960"))
	{
		GamePlayStatic::ScreenResize(Vec2(1910.f, 960.f), IsWindowMode);
	}

	if (ImGui::MenuItem("1920 X 1080"))
	{
		GamePlayStatic::ScreenResize(Vec2(1920.f, 1080.f), IsWindowMode);
	}

	if (ImGui::MenuItem("2560 X 1440"))
	{
		GamePlayStatic::ScreenResize(Vec2(2560.f, 1440.f), IsWindowMode);
	}
}

void SettingUI::Sound()
{
	static float fMatserVolume = CSound::GetMasterVolume() * 100.f;
	if (ImGui::SliderFloat("Master", &fMatserVolume, 0.f, 100.f))
	{
		CSound::SetMasterVolume(fMatserVolume / 100.f);
	}
}

#include <Engine\CRenderMgr.h>

void SettingUI::Graphic()
{
	static bool SmoothShadow = false;
	if (ImGui::CollapsingHeader("shadow"))
	{
		if (ImGui::Checkbox("On", &CRenderMgr::GetInst()->m_bGlobalShadowRender))
		{
		}
		if (ImGui::Checkbox("PCF On", &CRenderMgr::GetInst()->m_bGlobalPCF))
		{
		}
	}

	static bool Bloom = false;
	if (ImGui::CollapsingHeader("Bloom"))
	{
		if (ImGui::Checkbox("Object On", &CRenderMgr::GetInst()->m_GlobalBloomInfo.BloomActivate))
		{
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Global On", &CRenderMgr::GetInst()->m_GlobalBloomInfo.GlobalBloom))
		{
		}
		ImGui::DragFloat("Threshold", &CRenderMgr::GetInst()->m_GlobalBloomInfo.GlbalBloomSetting.fThreshold, 0.01f,
						 0.f, 1.f, "%.2f");

		ImGui::ColorEdit4("Global Color", CRenderMgr::GetInst()->m_GlobalBloomInfo.GlbalBloomSetting.vBloomColor);

		ImGui::DragInt("BlurCnt", &CRenderMgr::GetInst()->m_GlobalBloomInfo.BlurLevel, 1, 0, MAXBLURLEVEL);
		ImGui::DragFloat("MergeRatio", &CRenderMgr::GetInst()->m_GlobalBloomInfo.Ratio, 0.01f, 0.f, 1.f, "%.2f");
	}

	static bool Outliner = false;
	if (ImGui::CollapsingHeader("Outliner"))
	{
	}
}

#define KEYBUTTON(key)                                                                                   \
	string key = string(#key) + " (" + ToString(magic_enum::enum_name(info.key)) + ")##" + string(#key); \
	if (ImGui::Button(key.c_str()))                                                                      \
	{                                                                                                    \
		lastClicked = &CPlayerController::##key;                                                         \
	}
void SettingUI::Control()
{
	static KEY* lastClicked = nullptr;

	auto info = CPlayerController::GetInfo();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
	if (ImGui::Button("Initilize"))
	{
		CPlayerController::Init();
	}
	ImGui::PopStyleColor(3);

	KEYBUTTON(Front);
	KEYBUTTON(Right);
	KEYBUTTON(Back);
	KEYBUTTON(Left);
	KEYBUTTON(Dash);
	KEYBUTTON(Attack);
	KEYBUTTON(Zoom);
	KEYBUTTON(Reload);
	KEYBUTTON(Cover);
	KEYBUTTON(Skill);
	KEYBUTTON(Jump);
	KEYBUTTON(Flip);

	ImGui::SliderFloat("Sensitivity", &CPlayerController::Sensitivity, 0.f, 10.f);

	if (lastClicked)
	{
		for (size_t i = 0; i < (INT)KEY::KEY_END; i++)
		{
			if (KEY_TAP_EDITOR((KEY)i))
			{
				*lastClicked = (KEY)i;
				lastClicked	 = nullptr;
			}
		}
	}
}
