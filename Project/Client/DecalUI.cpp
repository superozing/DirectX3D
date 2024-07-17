#include "pch.h"
#include "DecalUI.h"

#include <Engine\CDecal.h>

DecalUI::DecalUI()
	: ComponentUI("Decal", "##Decal", COMPONENT_TYPE::DECAL)
	, strEmissiveCondition(" ")
{
	SetComponentTitle("Decal");
	strEmissiveCondition = "Off";
}

DecalUI::~DecalUI()
{
}

void DecalUI::render_update()
{
	ComponentUI::render_update();

	if (!TitleCollapse("Decal"))
		return;

	if (ImGui::Button(strEmissiveCondition.c_str(), ImVec2(50.f, 50.f)))
	{
		int iEmissive = GetTargetObject()->Decal()->GetDecalAsEmissive();

		if (iEmissive > 0)
		{
			strEmissiveCondition = "Off";
			GetTargetObject()->Decal()->SetDecalAsEmissive(0);
		}
		else
		{
			strEmissiveCondition = "On";
			GetTargetObject()->Decal()->SetDecalAsEmissive(1);
		}
	}
}