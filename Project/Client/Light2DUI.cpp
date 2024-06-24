#include "pch.h"
#include "Light2DUI.h"

#include <Engine\CLight2D.h>

Light2DUI::Light2DUI()
	: ComponentUI("Light2D", "##Light2D", COMPONENT_TYPE::LIGHT2D)
{
	SetComponentTitle("Light2D");
}

Light2DUI::~Light2DUI()
{
}

void Light2DUI::render_update()
{
	ComponentUI::render_update();

	if (!TitleCollapse("Light2D"))
		return;

	tLightInfo info = GetTargetObject()->Light2D()->GetLightInfo();

	int	  LightType = (int)info.LightType;
	Vec3  vColor	= info.vColor;
	Vec3  vAmbient	= info.vAmbient;
	Vec3  vWorldDir = info.vWorldDir;
	float fRadius	= info.fRadius;
	float fAngle	= info.fAngle;

	float Dir = 1.f;
	vWorldDir = AngleToVector(Dir);

	ImGui::Text("Light Type");
	ImGui::SameLine(0, 20);
	ImGui::PushItemWidth(150);
	ImGui::Combo("##LightType", &LightType, "DIRECTIONAL\0POINT\0SPOT\0\0");

	if (0 == LightType)
	{
		ImGui::Text("Ambient");
		ImGui::SameLine(0, 41);
		ImGui::PushItemWidth(250);
		ImGui::ColorEdit3("##LightAmbient", vAmbient);
	}
	else if (1 == LightType)
	{
		ImGui::Text("Light Color");
		ImGui::SameLine(0, 13);
		ImGui::PushItemWidth(200);
		ImGui::ColorPicker3("##LightColor", vColor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

		ImGui::Text("Radius");
		ImGui::SameLine(0, 48);
		ImGui::PushItemWidth(200);
		ImGui::DragFloat("##LightRadius", &fRadius);
	}
	else
	{
		ImGui::Text("Light Color");
		ImGui::SameLine(0, 13);
		ImGui::PushItemWidth(200);
		ImGui::ColorPicker3("##LightColor", vColor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

		ImGui::Text("Radius");
		ImGui::SameLine(0, 48);
		ImGui::PushItemWidth(200);
		ImGui::DragFloat("##LightRadius", &fRadius);

		ImGui::Text("LightDir");
		ImGui::SameLine(0, 34);
		ImGui::PushItemWidth(200);
		ImGui::SliderAngle("##LightDir", &Dir, 0.f, 360.f);

		ImGui::Text("Angle");
		ImGui::SameLine(0, 55);
		ImGui::PushItemWidth(200);
		ImGui::SliderAngle("##LightAngle", &fAngle, 0.f, 360.f);
	}

	GetTargetObject()->Light2D()->SetLightType((LIGHT_TYPE)LightType);
	GetTargetObject()->Light2D()->SetLightColor(vColor);
	GetTargetObject()->Light2D()->SetAmbient(vAmbient);
	GetTargetObject()->Light2D()->SetDir(vWorldDir);
	GetTargetObject()->Light2D()->SetRadius(fRadius);
	GetTargetObject()->Light2D()->SetAngle(fAngle);
}

Vec3 Light2DUI::AngleToVector(float _angle)
{
	float x = cosf(_angle);
	float y = sinf(_angle);

	return Vec3(x, y, 0.f);
}