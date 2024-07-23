#include "pch.h"
#include "PhysXUI.h"
#include "Engine\CPhysX.h"
#include "Engine\CTransform.h"

PhysXUI::PhysXUI()
	: ComponentUI("PhysX", "##PhysX", COMPONENT_TYPE::PHYSX)
{
	SetComponentTitle("PhysX");
}

PhysXUI::~PhysXUI()
{
}

void PhysXUI::render_update()
{
	ComponentUI::render_update();

	if (!TitleCollapse("PhysX"))
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

	auto Shape	  = phys->m_Shape;
	auto strShape = ToString(magic_enum::enum_name<PhysShape>(Shape));

	auto BodyType = phys->m_bPhysBodyType;
	auto strType  = ToString(magic_enum::enum_name<PhysBodyType>(BodyType));

	if (ImGui::BeginCombo("##BodyType", strType.c_str()))
	{
		for (size_t i = 0; i < (UINT)PhysBodyType::END; ++i)
		{
			bool isSelected = (i == (UINT)BodyType);
			auto TypeName	= ToString(magic_enum::enum_name<PhysBodyType>((PhysBodyType)i));
			if (ImGui::Selectable(TypeName.c_str(), isSelected))
			{
				phys->m_bPhysBodyType = (PhysBodyType)i;
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

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

	Vec3 Scale = phys->m_vScale;
	if (Vec3() == Scale && nullptr != phys->Transform())
	{
		Scale = phys->Transform()->GetRelativeScale();
	}
	ImGui::DragFloat3("Scale", (float*)Scale);
	phys->m_vScale = Scale;

	Vec3 OffsetPos = phys->m_vOffsetPos;
	ImGui::DragFloat3("OffsetPos", (float*)OffsetPos);
	phys->m_vOffsetPos = OffsetPos;

	phys->m_bImguiDirtyFlag	   = true;
	const auto& vecContactInfo = phys->m_vThisFrameContact;

	ImGui::Text(("Col Cnt:" + std::to_string(phys->m_CollisionCount)).c_str());

	for (const auto& info : vecContactInfo)
	{
		ImGui::Text(ToString(info.Other->GetName()).c_str());
	}
}
