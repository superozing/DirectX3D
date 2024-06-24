#include "pch.h"
#include "Animator2DUI.h"

Animator2DUI::Animator2DUI()
	: ComponentUI("Animator2D", "##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
{
	SetComponentTitle("Animator2D");
}

Animator2DUI::~Animator2DUI()
{
}

void Animator2DUI::render_update()
{
	ComponentUI::render_update();

	if (!TitleCollapse("Animator2D"))
		return;
}