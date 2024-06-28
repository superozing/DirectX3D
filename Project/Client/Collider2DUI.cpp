#include "pch.h"
#include "Collider2DUI.h"

#include <Engine\CCollider2D.h>

Collider2DUI::Collider2DUI()
	: ComponentUI("Collider2D", "##Collider2D", COMPONENT_TYPE::COLLIDER2D)
{
	SetComponentTitle("Collider2D");
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::render_update()
{
	ComponentUI::render_update();

	if (!TitleCollapse("Collider2D"))
		return;

	Vec2		vPos	  = GetTargetObject()->Collider2D()->GetOffsetPos();
	Vec2		vScale	  = GetTargetObject()->Collider2D()->GetOffsetScale();
	static int	ColType	  = int(GetTargetObject()->Collider2D()->GetType());
	static bool linkScale = false;

	static float OriginX = 1.f;
	static float OriginY = 1.f;
	static float NewX	 = 1.f;
	static float NewY	 = 1.f;

	OriginX = vScale.x;
	OriginY = vScale.y;

	ImGui::Text("Type");
	ImGui::SameLine(0, 78);
	ImGui::PushItemWidth(80);
	ImGui::Combo("##ColType", &ColType, "RECT\0CIRCLE\0\0");

	ImGui::Text("Position");
	ImGui::SameLine(0, 35);
	ImGui::PushItemWidth(80);
	ImGui::Text("x");
	ImGui::SameLine();
	ImGui::DragFloat("##Posx", &vPos.x);
	ImGui::SameLine();
	ImGui::Text("y");
	ImGui::SameLine();
	ImGui::DragFloat("##Posy", &vPos.y);

	if ((int)COLLIDER2D_TYPE::RECT == ColType)
	{
		ImGui::Text("Scale");
		ImGui::SameLine(0, 56);
		ImGui::PushItemWidth(80);
		ImGui::Text("x");
		ImGui::SameLine();
		ImGui::DragFloat("##Scalex", &vScale.x);
		ImGui::SameLine();
		ImGui::Text("y");
		ImGui::SameLine();
		ImGui::DragFloat("##Scaley", &vScale.y);
		ImGui::SameLine();
		ImGui::Checkbox("Link Scale", &linkScale);

		if (linkScale)
		{
			NewX = vScale.x;
			NewY = vScale.y;

			if (0 != vScale.x && 0 != vScale.y)
			{
				if (1 > OriginX / NewX || 1 < OriginX / NewX)
				{
					float ratioX = NewX / OriginX;

					vScale.y *= ratioX;

					NewX = OriginX;
				}
				else if (1 > OriginY / NewY || 1 < OriginY / NewY)
				{
					float ratioY = NewY / OriginY;

					vScale.x *= ratioY;

					NewY = OriginY;
				}
			}
		}
	}
	else
	{
		ImGui::Text("Radius");
		ImGui::SameLine(0, 64);
		ImGui::PushItemWidth(80);
		ImGui::DragFloat("##Radius", &vScale.x);

		vScale.y = vScale.x;
	}

	GetTargetObject()->Collider2D()->SetOffsetPos(vPos);
	GetTargetObject()->Collider2D()->SetOffsetScale(vScale);
	GetTargetObject()->Collider2D()->SetColliderType((COLLIDER2D_TYPE)ColType);

	// 충돌체 렌더 여부
	bool b = GetTargetObject()->Collider2D()->IsDebug();
	ImGui::Text("Show Collider");
	ImGui::SameLine(0, 15);
	ImGui::Checkbox("##ShowCol", &b);
	GetTargetObject()->Collider2D()->SetDebug(b);

	// Absolute 체크
	bool bAbsolute = GetTargetObject()->Collider2D()->IsAbsolute();
	ImGui::Text("Scale Absolute");
	ImGui::SameLine();
	ImGui::Checkbox("##Collider2DAbsolute", &bAbsolute);
	GetTargetObject()->Collider2D()->SetAbsolute(bAbsolute);
}