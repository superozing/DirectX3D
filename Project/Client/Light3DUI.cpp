#include "pch.h"
#include "Light3DUI.h"

#include <Engine\CLight3D.h>


Light3DUI::Light3DUI()
	: ComponentUI("Light3D", "##Light3D", COMPONENT_TYPE::LIGHT3D)
{
	SetSize(ImVec2(0.f, 430.f));
	SetComponentTitle("LIGHT3D");
}

Light3DUI::~Light3DUI()
{
}

void Light3DUI::render_update()
{
	ComponentUI::render_update();

	if (!TitleCollapse("Light3D")) return;

	tLightInfo info = GetTargetObject()->Light3D()->GetLightInfo();

	int LightType = (int)info.LightType;
	Vec3 vColor = info.vColor;
	Vec3 vAmbient = info.vAmbient;
	float fRadius = info.fRadius;
	float fAngle = info.fAngle;
	Vec3 vWorldDir = info.vWorldDir;
	Vec3 vSpecular = info.vSpecular;
	vWorldDir.ToDegree();

	ImGui::Text("Light Type");
	ImGui::SameLine(0, 20); ImGui::PushItemWidth(150);
	ImGui::Combo("##LightType", &LightType, "DIRECTIONAL\0POINT\0SPOT\0\0");

	if (0 == LightType)
	{
		ImGui::Text("Light Color"); ImGui::SameLine(); ImGui::PushItemWidth(200.f);
		ImGui::ColorPicker3("##LightColor", vColor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

		ImGui::Text("Specular   "); ImGui::SameLine(); ImGui::PushItemWidth(200.f);
		ImGui::ColorPicker3("##Specular", vSpecular, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

		ImGui::Text("Ambient    ");	ImGui::SameLine(); ImGui::PushItemWidth(200.f);
		ImGui::ColorEdit3("##LightAmbient", vAmbient);
	}
	else if (1 == LightType)
	{
		ImGui::Text("Light Color"); ImGui::SameLine(); ImGui::PushItemWidth(200.f);
		ImGui::ColorPicker3("##LightColor", vColor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

		ImGui::Text("Specular   "); ImGui::SameLine(); ImGui::PushItemWidth(200.f);
		ImGui::ColorPicker3("##Specular", vSpecular, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

		ImGui::Text("Ambient    ");	ImGui::SameLine(); ImGui::PushItemWidth(200.f);
		ImGui::ColorEdit3("##LightAmbient", vAmbient);

		ImGui::Text("Radius     "); ImGui::SameLine(); ImGui::PushItemWidth(200.f);
		ImGui::DragFloat("##LightRadius", &fRadius, 0.1f, 0.f, 0.f, "%.1f");
	}
	else
	{
		ImGui::Text("Light Color"); ImGui::SameLine(); ImGui::PushItemWidth(200.f);
		ImGui::ColorPicker3("##LightColor", vColor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

		ImGui::Text("Specular   "); ImGui::SameLine(); ImGui::PushItemWidth(200.f);
		ImGui::ColorPicker3("##Specular", vSpecular, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

		ImGui::Text("Ambient    ");	ImGui::SameLine(); ImGui::PushItemWidth(200.f);
		ImGui::ColorEdit3("##LightAmbient", vAmbient);

		ImGui::Text("Radius     "); ImGui::SameLine(); ImGui::PushItemWidth(200.f);
		ImGui::DragFloat("##LightRadius", &fRadius, 0.1f, 0.f, 0.f, "%.1f");

		ImGui::Text("LightDir   "); ImGui::SameLine(); ImGui::PushItemWidth(200.f);
		ImGui::DragFloat3("##LightDir", vWorldDir, 0.01f, -1.f, 1.f, "%.2f");

		ImGui::Text("Angle      "); ImGui::SameLine(); ImGui::PushItemWidth(200.f);
		ImGui::SliderAngle("##LightAngle", &fAngle, 0.f, 179.f);
	
		vWorldDir.ToRadian();
	}

	GetTargetObject()->Light3D()->SetLightType((LIGHT_TYPE)LightType);
	GetTargetObject()->Light3D()->SetLightColor(vColor);
	GetTargetObject()->Light3D()->SetAmbient(vAmbient);
	GetTargetObject()->Light3D()->SetRadius(fRadius);
	GetTargetObject()->Light3D()->SetSpecular(vSpecular);
	GetTargetObject()->Light3D()->SetLightDir(vWorldDir);
	GetTargetObject()->Light3D()->SetAngle(fAngle);
}