#include "pch.h"
#include "PhysXUI.h"
#include "Engine\CPhysX.h"

PhysXUI::PhysXUI()
	: ComponentUI("PhysXUI", "##PhysXUI", COMPONENT_TYPE::PHYSX)
{
	SetComponentTitle("PhysX");
}

PhysXUI::~PhysXUI()
{
}

void PhysXUI::render_update()
{
	ComponentUI::render_update();

	if (!TitleCollapse("PhysXUI")) return;

	auto TGO = GetTargetObject();
	if (nullptr == TGO)
	{
		return;
	}
	auto phys = TGO->PhysX();
	if (nullptr == phys)
	{
		return;
	}

	phys->m_bImguiDirtyFlag = true;
	const auto& vecContactInfo = phys->m_vThisFrameContact;

	ImGui::Text(("Col Cnt:" + std::to_string( phys->m_CollisionCount)).c_str());

	for (const auto& info : vecContactInfo)
	{
		ImGui::Text(ToString( info.Other->GetName()).c_str());
	}

}

