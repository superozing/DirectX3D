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

	if (!TitleCollapse("Animator3D") || !GetTargetObject()->Animator3D())
		return;

	auto AnimClip = GetTargetObject()->Animator3D()->GetAnimClip();
	if (AnimClip->size() <= 0)
		return;
	int	  CurAnimIdx = GetTargetObject()->Animator3D()->GetCurClip();
	int	  iLoopCount = GetTargetObject()->Animator3D()->GetLoopCount();
	float ClipTime	 = GetTargetObject()->Animator3D()->GetClipUpdateTime(CurAnimIdx);

	string CurAnimName = ToString(AnimClip->at(CurAnimIdx).strAnimName);
	float  fEndTime	   = (float)AnimClip->at(CurAnimIdx).dEndTime;

	ImGui::Text("AnimClip ");
	ImGui::SameLine();
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

	ImGui::Text("LoopCount");
	ImGui::SameLine();
	ImGui::DragInt("##LoopCount", &iLoopCount);
	if (iLoopCount < -1)
		iLoopCount = -1;

	ImGui::Text("ClipTime ");
	ImGui::SameLine();
	ImGui::SliderFloat("##AnimTime", &ClipTime, 0.f, fEndTime, "%.2f");

	GetTargetObject()->Animator3D()->SetCurClip(CurAnimIdx);
	GetTargetObject()->Animator3D()->SetLoopCount(iLoopCount);

	if (!(GetTargetObject()->Animator3D()->IsPlayable()))
		GetTargetObject()->Animator3D()->SetClipTime(CurAnimIdx, ClipTime);

	if (ImGui::Button("Play"))
		GetTargetObject()->Animator3D()->Play(CurAnimIdx, iLoopCount);
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
		GetTargetObject()->Animator3D()->Stop();
}
