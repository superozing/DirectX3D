#include "pch.h"
#include "Animator3DUI.h"

#include <Engine\CAnimator3D.h>

Animator3DUI::Animator3DUI()
	: ComponentUI("Animator3D", "##Animator3D", COMPONENT_TYPE::ANIMATOR3D)
{
	SetComponentTitle("Animator3D");
}

Animator3DUI::~Animator3DUI()
{
}

void Animator3DUI::render_update()
{
	ComponentUI::render_update();

	if (!TitleCollapse("Animator3D")) return;

	auto AnimClip = GetTargetObject()->Animator3D()->GetAnimClip();
	int CurAnimIdx = GetTargetObject()->Animator3D()->GetCurClip();
	bool bPlayable = GetTargetObject()->Animator3D()->IsPlayable();
	int iLoopCount = GetTargetObject()->Animator3D()->GetLoopCount();
	float ClipTime = GetTargetObject()->Animator3D()->GetClipUpdateTime(CurAnimIdx);

	string CurAnimName = ToString(AnimClip->at(CurAnimIdx).strAnimName);

	ImGui::Text("AnimClip");
	if (ImGui::BeginCombo("##AnimClip", CurAnimName.c_str()))
	{
		for (size_t i = 0; i < AnimClip->size(); ++i)
		{
			bool bSelected = (CurAnimIdx == i);
			
			if (ImGui::Selectable(ToString(AnimClip->at(i).strAnimName).c_str(), bSelected))
			{
				CurAnimIdx = i;
			}

			if (bSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::Text("AnimPlay");
	ImGui::Checkbox("##AnimPlay", &bPlayable);
	
	ImGui::Text("LoopCount");
	ImGui::DragInt("##LoopCount", &iLoopCount);
	if (iLoopCount < -1)
		iLoopCount = -1;

	GetTargetObject()->Animator3D()->SetCurClip(CurAnimIdx);
	GetTargetObject()->Animator3D()->SetPlayable(bPlayable);
	GetTargetObject()->Animator3D()->SetLoopCount(iLoopCount);
	
	if (!bPlayable)
		GetTargetObject()->Animator3D()->SetClipTime(CurAnimIdx, ClipTime);
}
