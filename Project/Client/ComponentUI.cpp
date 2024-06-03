﻿#include "pch.h"
#include "ComponentUI.h"

ComponentUI::ComponentUI(const string& _strName, const string& _ID, COMPONENT_TYPE _Type)
	: UI(_strName, _ID)
	, m_TargetObject(nullptr)
	, m_Type(_Type)
{
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::render_update()
{
	ImGui::Separator();

	ImGui::PushID(0);

	ImGui::PopID();
}

void ComponentUI::ResetUIinfo()
{
}

void ComponentUI::SetTargetObject(CGameObject* _Target)
{
	m_TargetObject = _Target;

	// 타겟 오브젝트가 없거나, 타겟 오브젝트가 컴포넌트UI 담당 컴포넌트를 보유하고 있지 않다며
	// 컴포넌트UI 를 비활성화 한다.
	if (nullptr == m_TargetObject)
	{
		Deactivate();
	}

	else
	{
		if (!m_TargetObject->GetComponent(m_Type))
		{
			Deactivate();
		}
		else
		{
			Activate();
		}
	}
}

void ComponentUI::CheckPayLoadData(ASSET_TYPE _Type, int iFuncArrNum, CGameObject* _TargetObj)
{
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			DWORD_PTR data = *((DWORD_PTR*)payload->Data);
			CAsset* pAsset = (CAsset*)data;

			// 등록된 함수 호출
			m_vecPayloadEvent[iFuncArrNum](pAsset, _Type, _TargetObj);

		}

		ImGui::EndDragDropTarget();
	}
}


void ComponentUI::AddPayLoadEvent(SetPayloadEvent _Func)
{

	m_vecPayloadEvent.push_back(_Func);

}
