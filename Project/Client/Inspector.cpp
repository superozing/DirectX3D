#include "pch.h"
#include "Inspector.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine/CTransform.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "Collider2DUI.h"
#include "Light2DUI.h"
#include "Animator2DUI.h"
#include "ScriptUI.h"

#include "AssetUI.h"


Inspector::Inspector()
	: UI("Inspector", "##Inspector")
	, m_TargetObject(nullptr)
	, m_arrComUI{}
{
	// 자식 UI 생성
	CreateChildUI();
}

Inspector::~Inspector()
{

}

void Inspector::tick()
{
}

void Inspector::render_update()
{
	if (nullptr == m_TargetObject)
		return;

	if (nullptr != m_TargetObject)
	{
		// 오브젝트 이름
		char ObjName[255] = {};
		string strName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
		string prevName = strName;

		for (size_t i = 0; i < strName.length(); ++i)
		{
			ObjName[i] = strName[i];
		}

		ImGui::SameLine();
		ImGui::InputText("##ObjName", ObjName, 255);
		m_TargetObject->SetName(ToWString(string(ObjName)));

		if (prevName != string(ObjName))
		{
			GamePlayStatic::ChangeName();
		}

		// 오브젝트 레이어
		int LayerIdx = m_TargetObject->GetLayerIdx();
		int PrevIdx = LayerIdx;

		if (-1 != LayerIdx)
		{
			ImGui::Text("Layer"); ImGui::SameLine();
			auto Layer_Names = magic_enum::enum_names<LAYER>();
			string strLayer = string(Layer_Names[LayerIdx]);

			if (ImGui::BeginCombo("##ObjLayer", strLayer.c_str()))
			{
				for (int i = 0; i < (int)Layer_Names.size(); ++i)
				{
					int CurLayer = i;

					bool isSelected = (CurLayer == LayerIdx);

					if (ImGui::Selectable(string(Layer_Names[CurLayer]).c_str(), isSelected))
					{
						LayerIdx = CurLayer;
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();

				if (PrevIdx != LayerIdx)
				{
					CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
					pCurLevel->AddObject(m_TargetObject, LayerIdx);
				}
			}
		}
	}
}

void Inspector::SetTargetObject(CGameObject* _Object)
{
	// Target 오브젝트 설정
	m_TargetObject = _Object;

	// 해당 오브젝트가 보유하고 있는 컴포넌트에 대응하는 컴포넌트UI 활성화
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
		{
			m_arrComUI[i]->SetTargetObject(_Object);
		}
	}

	RefreshScriptUI();


	// AssetUI 비활성화
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->Deactivate();
	}
}

void Inspector::SetTargetAsset(Ptr<CAsset> _Asset)
{
	SetTargetObject(nullptr);

	m_TargetAsset = _Asset;

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->Deactivate();
	}
		
	if(nullptr != m_TargetAsset)
	{
		m_arrAssetUI[(UINT)m_TargetAsset->GetType()]->Activate();
		m_arrAssetUI[(UINT)m_TargetAsset->GetType()]->SetAsset(_Asset);
	}	
}
