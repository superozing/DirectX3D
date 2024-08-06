#include "pch.h"
#include "SettingUI.h"

#include "MenuUI.h"

#include <Engine\CDevice.h>
#include <Engine\CSound.h>
#include <Engine\CKeyMgr.h>

#include <Scripts\CPlayerController.h>
#include <Scripts\CButtons.h>
void SettingActive()
{
	CImGuiMgr::GetInst()->FindUI("##Setting")->Activate();
}
SettingUI::SettingUI()
	: UI("Setting", "##Setting")
{
	CButtons::Setting = &(SettingActive);
}

SettingUI::~SettingUI()
{
}

void SettingUI::tick()
{
}

void SettingUI::render_update()
{
	Display();
	Sound();
	Graphic();
	Control();
}

void SettingUI::Display()
{
	int ResolutionX = int(CDevice::GetInst()->GetRenderResolution().x);
	int ResolutionY = int(CDevice::GetInst()->GetRenderResolution().y);

	string Resolution = to_string(ResolutionX) + " X " + to_string(ResolutionY);

	if (ImGui::BeginMenu("Display"))
	{
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

		ImGui::EndMenu();
	}
}

void SettingUI::Sound()
{
	if (ImGui::BeginMenu("Sound"))
	{
		static float fMatserVolume = CSound::GetMasterVolume() * 100.f;
		if (ImGui::SliderFloat("Master", &fMatserVolume, 0.f, 100.f))
		{
			CSound::SetMasterVolume(fMatserVolume / 100.f);
		}

		ImGui::EndMenu();
	}
}

void SettingUI::Graphic()
{
	if (ImGui::BeginMenu("Graphic"))
	{
		static bool shadow = false;
		if (ImGui::Checkbox("Shadow", &shadow))
		{
		}

		static bool SmoothShadow = false;
		if (ImGui::Checkbox("SmoothShadow", &SmoothShadow))
		{
		}

		static bool Emissive = false;
		if (ImGui::Checkbox("Emissive", &Emissive))
		{
		}

		static bool Bloom = false;
		if (ImGui::Checkbox("Bloom", &Bloom))
		{
		}

		static bool Outliner = false;
		if (ImGui::Checkbox("Outliner", &Outliner))
		{
		}

		ImGui::EndMenu();
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

	if (ImGui::BeginMenu("Control"))
	{
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

		ImGui::EndMenu();
	}
	else
	{
		lastClicked = nullptr;
	}
}
