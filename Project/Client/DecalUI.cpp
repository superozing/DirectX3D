#include "pch.h"
#include "DecalUI.h"

DecalUI::DecalUI()
	: ComponentUI("Decal", "##Decal", COMPONENT_TYPE::DECAL)
{
	SetComponentTitle("Decal");
}

DecalUI::~DecalUI()
{
}

void DecalUI::render_update()
{
	ComponentUI::render_update();

	if (!TitleCollapse("Decal"))
		return;
}