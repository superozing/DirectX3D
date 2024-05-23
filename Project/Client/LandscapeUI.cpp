#include "pch.h"
#include "LandscapeUI.h"

#include <Engine\CLandScape.h>


LandScapeUI::LandScapeUI()
	: ComponentUI("LandScapeUI", "##LandScapeUI", COMPONENT_TYPE::LANDSCAPE)
{
}

LandScapeUI::~LandScapeUI()
{
}

void LandScapeUI::render_update()
{
	if (!TitleCollapse("LandScape")) return;
}