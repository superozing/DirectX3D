#include "pch.h"
#include "TransformUI.h"

#include <Engine/CTransform.h>

TransformUI::TransformUI()
	: ComponentUI("Transform", "##Transform", COMPONENT_TYPE::TRANSFORM)
{
	SetComponentTitle("Transform");
}

TransformUI::~TransformUI()
{
}

void TransformUI::CopyPaste(const string& _id, Vec3& _vec)
{
	static Vec3 vClipBoard = Vec3();
	static Vec3 vReturn	   = Vec3();

	ImGui::SameLine();
	string id;
	id = "C##Transform" + _id;
	if (ImGui::Button(id.c_str()))
	{
		vClipBoard = _vec;
	}

	ImGui::SameLine();
	id = "P##Transform" + _id;
	if (ImGui::Button(id.c_str()))
	{
		vReturn = _vec;
		_vec	= vClipBoard;
	}

	ImGui::SameLine();
	id = "R##Transform" + _id;
	if (ImGui::Button(id.c_str()))
	{
		Vec3 temp = _vec;
		_vec	  = vReturn;
		vReturn	  = temp;
	}
}

void TransformUI::render_update()
{
	ComponentUI::render_update();

	if (!TitleCollapse("Transform"))
		return;

	Vec3 vPos	= GetTargetObject()->Transform()->GetRelativePos();
	Vec3 vScale = GetTargetObject()->Transform()->GetRelativeScale();
	Vec3 vRot	= GetTargetObject()->Transform()->GetRelativeRotation();
	vRot.ToDegree();

	ImGui::Text("Position");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Position", vPos);
	CopyPaste("Pos", vPos);

	ImGui::Text("Scale   ");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Scale", vScale);
	CopyPaste("Scale", vScale);

	ImGui::Text("Rotation");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Rotation", vRot);
	CopyPaste("Rotation", vRot);

	vRot.ToRadian();
	GetTargetObject()->Transform()->SetRelativePos(vPos);
	GetTargetObject()->Transform()->SetRelativeScale(vScale);
	GetTargetObject()->Transform()->SetRelativeRotation(vRot);

	// Absolute 값
	bool bAbsolute = GetTargetObject()->Transform()->IsAbsolute();
	ImGui::Text("Scale Absolute");
	ImGui::SameLine();
	ImGui::Checkbox("##TransformAbsolute", &bAbsolute);
	GetTargetObject()->Transform()->SetAbsolute(bAbsolute);
}
