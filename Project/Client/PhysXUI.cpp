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

	if (!TitleCollapse("PhysXUI"))
		return;

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

	static auto Shape	 = phys->m_Shape;
	auto		strShape = ToString(magic_enum::enum_name<PhysShape>(Shape));

	if (ImGui::BeginCombo("##Shape", strShape.c_str()))
	{
		for (size_t i = 0; i < (UINT)PhysShape::END; ++i)
		{
			bool isSelected = (i == (UINT)Shape);
			auto ShapeName	= ToString(magic_enum::enum_name<PhysShape>((PhysShape)i));
			if (ImGui::Selectable(ShapeName.c_str(), isSelected))
			{
				phys->m_Shape = (PhysShape)i;
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

	static auto IsStatic = phys->m_bStaticActor;
	if (ImGui::Checkbox("IsStatic", &IsStatic))
	{
		phys->m_bStaticActor = IsStatic;
	}

	phys->m_bImguiDirtyFlag	   = true;
	const auto& vecContactInfo = phys->m_vThisFrameContact;

	ImGui::Text(("Col Cnt:" + std::to_string(phys->m_CollisionCount)).c_str());

	for (const auto& info : vecContactInfo)
	{
		ImGui::Text(ToString(info.Other->GetName()).c_str());
	}
}
