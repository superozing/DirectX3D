﻿#include "pch.h"
#include "Inspector.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine/components.h>
#include <Engine/CRenderMgr.h>

#include "CImGuiMgr.h"
#include "Outliner.h"

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "Collider2DUI.h"
#include "Light2DUI.h"
#include "Animator2DUI.h"
#include "ScriptUI.h"
#include "SkyBoxUI.h"
#include "ParticleSystemUI.h"

#include "AssetUI.h"
#include "ModelUI.h"
#include "Content.h"

void Inspector::ResetTargetObject()
{
	SetTargetObject(nullptr);
}

void Inspector::ResetTargetAsset()
{
	SetTargetAsset(nullptr);
}

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
		ObjectName();

		if (!m_bPrefab)
		{
			ImGui::SameLine();
			if (ImGui::Button("Save Prefab"))
			{
				SavePrefab();
			}

			ObjectLayer();
		}
		else
		{
			ImGui::SameLine();
			if (ImGui::Button("Save Prefab"))
			{
				while (m_TargetObject->GetParent())
				{
					SetTargetObject(m_TargetObject->GetParent(), true);
				}
				SavePrefab();
			}

			PrefabLayer();
			ImGui::SameLine();

			if (ImGui::Button("Spawn Prefab"))
			{
				auto Target			= m_TargetObject->Clone();
				int	 idx			= Target->GetLayerIdx();
				Target->m_iLayerIdx = -1;
				GamePlayStatic::SpawnGameObject(Target, idx);

				auto cam	= CRenderMgr::GetInst()->GetMainCam();
				auto vPos	= cam->Transform()->GetRelativePos();
				auto vFront = cam->Transform()->GetWorldDir(DIR_TYPE::FRONT);
				vPos += vFront * 100.f;
				Target->Transform()->SetRelativePos(vPos);
			}

			if (ImGui::Button("parent"))
			{
				if (m_TargetObject->GetParent())
					SetTargetObject(m_TargetObject->GetParent(), true);
			}

			if (ImGui::CollapsingHeader("Child List"))
			{
				for (size_t i = 0; i < m_TargetObject->m_vecChild.size(); i++)
				{
					string id =
						ToString(m_TargetObject->m_vecChild[i]->GetName()) + "##childbutton" + std::to_string(i);
					if (ImGui::Button(id.c_str()))
					{
						SetTargetObject(m_TargetObject->m_vecChild[i], true);
					}
				}
			}
		}

		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("ComponentList");
		}

		if (ImGui::BeginPopup("ComponentList"))
		{
			ObjectComponent();

			ImGui::EndPopup();
		}

		ImGui::Separator();

		ObjectScript();
	}
}

void Inspector::enter()
{
	ResetTargetObject();
	ResetTargetAsset();
}

void Inspector::SetTargetObject(CGameObject* _Object, bool _bPrefab)
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

	auto mUI = (ModelUI*)CImGuiMgr::GetInst()->FindUI("##ModelUI");
	if (m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR3D] && !_bPrefab)
		mUI->SetModel(_Object);

	RefreshScriptUI();

	// AssetUI 비활성화
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->Deactivate();
	}

	m_bPrefab = _bPrefab;
}

void Inspector::SetTargetAsset(Ptr<CAsset> _Asset)
{
	SetTargetObject(nullptr);

	m_TargetAsset = _Asset;

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->Deactivate();
	}

	if (nullptr != m_TargetAsset)
	{
		m_arrAssetUI[(UINT)m_TargetAsset->GetType()]->Activate();
		m_arrAssetUI[(UINT)m_TargetAsset->GetType()]->SetAsset(_Asset);
	}
}

void Inspector::ObjectName()
{
	// 오브젝트 이름
	char   ObjName[255] = {};
	string strName		= string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
	string prevName		= strName;

	for (size_t i = 0; i < strName.length(); ++i)
	{
		ObjName[i] = strName[i];
	}

	ImGui::SameLine();
	ImGui::InputText("##ObjName", ObjName, 255);

	if (prevName != string(ObjName))
	{
		if (KEY_TAP_EDITOR(ENTER))
		{
			m_TargetObject->SetName(ToWString(string(ObjName)));
			prevName = ObjName;

			Outliner* pOutliner = (Outliner*)CImGuiMgr::GetInst()->FindUI("##Outliner");
			pOutliner->ResetCurrentLevel();
		}
		else if (KEY_TAP_EDITOR(ESC))
		{
			m_TargetObject->SetName(ToWString(string(strName)));
		}
	}
}

void Inspector::ObjectLayer()
{
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	// 오브젝트 레이어
	int LayerIdx = m_TargetObject->GetLayerIdx();
	int PrevIdx	 = LayerIdx;

	if (-1 != LayerIdx)
	{
		string LayerName = ToString(CurLevel->GetLayer(LayerIdx)->GetName());
		ImGui::Text("Layer");
		ImGui::SameLine();
		auto LayerVal = magic_enum::enum_cast<LAYER>(LayerName);

		string strLayer = LayerName;

		static auto mLayer = GamePlayStatic::GetLayerMap();

		if (ImGui::BeginCombo("##ObjLayer", strLayer.c_str()))
		{
			for (size_t i = 0; i < mLayer.size(); ++i)
			{
				size_t CurLayer		= mLayer[i].LayerIdx;
				string CurLayerName = mLayer[i].LayerName;

				if (!magic_enum::enum_cast<LAYER>(CurLayerName).has_value())
					continue;

				bool isSelected = (CurLayer == LayerIdx);

				if (ImGui::Selectable(CurLayerName.c_str(), isSelected))
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
				CurLevel->ChangeLayer(m_TargetObject, LayerIdx, false);
			}
		}
	}
}

int Inspector::PrefabLayer()
{
	int LayerIdx = m_TargetObject->GetLayerIdx();
	int PrevIdx	 = LayerIdx;

	if (LayerIdx == -1)
		m_TargetObject->m_iLayerIdx = 0;

	LayerIdx = m_TargetObject->GetLayerIdx();

	ImGui::Text("Layer");
	ImGui::SameLine();

	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	string strLayer = ToString(CurLevel->GetLayer(LayerIdx)->GetName());

	static auto mLayer = GamePlayStatic::GetLayerMap();
	if (ImGui::BeginCombo("##ObjLayer", strLayer.c_str()))
	{
		for (size_t i = 0; i < mLayer.size(); ++i)
		{
			size_t CurLayer		= mLayer[i].LayerIdx;
			string CurLayerName = mLayer[i].LayerName;

			if (!magic_enum::enum_cast<LAYER>(CurLayerName).has_value())
				continue;

			bool isSelected = (CurLayer == LayerIdx);

			if (ImGui::Selectable(CurLayerName.c_str(), isSelected))
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
			// CurLevel->AddObject(m_TargetObject, LayerIdx);
			m_TargetObject->m_iLayerIdx = LayerIdx;
			PrevIdx						= LayerIdx;
		}
	}

	return LayerIdx;
}

void Inspector::ObjectComponent()
{
	auto ComponentList = magic_enum::enum_names<COMPONENT_TYPE>();

	for (size_t i = 0; i < ComponentList.size() - 2; ++i)
	{
		if (ImGui::MenuItem(string(ComponentList[i]).c_str()))
		{
			CheckTargetComponent((COMPONENT_TYPE)i);
		}
	}
}

void Inspector::ObjectScript()
{
	static int			   CurSciprt = 0;
	static ImGuiTextFilter filter;
	vector<string>		   filteredScripts;
	ImGui::Text("Script Filter");
	ImGui::SameLine();
	filter.Draw("##Script Filter");

	auto ScriptList = magic_enum::enum_names<SCRIPT_TYPE>();
	for (const auto& script : ScriptList)
	{
		// PassFilter : filter에 입력된 문자열과 비교하여 현재 텍스트가 필터를 통과하는지 확인하는
		// 함수
		if (filter.PassFilter(script.data()))
		{
			filteredScripts.push_back(string(script.data()));
		}
	}

	if (CurSciprt >= filteredScripts.size())
		CurSciprt = 0;
	if (0 == filteredScripts.size())
		CurSciprt = -1;

	if (-1 != CurSciprt)
	{
		string strScript = filteredScripts[CurSciprt];

		if (ImGui::BeginCombo("##ScriptList", strScript.c_str()))
		{
			for (int i = 0; i < filteredScripts.size(); ++i)
			{
				bool is_selected = (CurSciprt == i);

				if (ImGui::Selectable(filteredScripts[i].c_str(), is_selected))
				{
					CurSciprt = i;
					strScript = filteredScripts[CurSciprt];
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();

		if (ImGui::Button("Add Script"))
		{
			if (!strScript.empty())
			{
				auto ScriptType = magic_enum::enum_cast<SCRIPT_TYPE>(strScript);
				if (ScriptType.has_value())
				{
					SCRIPT_TYPE type = ScriptType.value();
					m_TargetObject->AddComponent(CScriptMgr::GetScript((UINT)type));
				}
			}

			SetTargetObject(GetTargetObject());
		}
	}
}

void Inspector::CheckTargetComponent(COMPONENT_TYPE _type)
{
	if (nullptr != m_TargetObject->GetComponent((COMPONENT_TYPE)_type))
	{
		MessageBoxA(nullptr, "Already contains the same component", "Can't add the same component multiple times!",
					MB_OK);
		return;
	}

	// 두 개 이상의 렌더 컴포넌트를 추가하려고 할 시
	if (CComponent::IsRenderComponent(_type))
	{
		if (nullptr != m_TargetObject->GetRenderComponent())
		{
			MessageBoxA(nullptr, "Already contains the other render component",
						"Can't add more than one render component!", MB_OK);
			return;
		}
	}

	switch ((COMPONENT_TYPE)_type)
	{
	case COMPONENT_TYPE::TRANSFORM:
		m_TargetObject->AddComponent(new CTransform);
		SetTargetObject(GetTargetObject());
		break;
	case COMPONENT_TYPE::COLLIDER2D:
		m_TargetObject->AddComponent(new CCollider2D);
		SetTargetObject(GetTargetObject());
		break;
	case COMPONENT_TYPE::COLLIDER3D:
		break;
	case COMPONENT_TYPE::ANIMATOR2D:
		m_TargetObject->AddComponent(new CAnimator2D);
		SetTargetObject(GetTargetObject());
		break;
	case COMPONENT_TYPE::ANIMATOR3D:
		m_TargetObject->AddComponent(new CAnimator3D);
		SetTargetObject(GetTargetObject());
		break;
	case COMPONENT_TYPE::LIGHT2D:
		m_TargetObject->AddComponent(new CLight2D);
		SetTargetObject(GetTargetObject());
		break;
	case COMPONENT_TYPE::LIGHT3D:
		m_TargetObject->AddComponent(new CLight3D);
		SetTargetObject(GetTargetObject());
		break;
	case COMPONENT_TYPE::CAMERA:
		m_TargetObject->AddComponent(new CCamera);
		SetTargetObject(GetTargetObject());
		break;
	case COMPONENT_TYPE::STATEMACHINE:
		break;
	case COMPONENT_TYPE::MESHRENDER:
		m_TargetObject->AddComponent(new CMeshRender);
		SetTargetObject(GetTargetObject());
		break;
	case COMPONENT_TYPE::TILEMAP:
		m_TargetObject->AddComponent(new CTileMap);
		SetTargetObject(GetTargetObject());
		break;
	case COMPONENT_TYPE::PARTICLESYSTEM:
		m_TargetObject->AddComponent(new CParticleSystem);
		SetTargetObject(GetTargetObject());
		break;
	case COMPONENT_TYPE::SKYBOX:
		m_TargetObject->AddComponent(new CSkyBox);
		SetTargetObject(GetTargetObject());
		break;
	case COMPONENT_TYPE::DECAL:
		m_TargetObject->AddComponent(new CDecal);
		SetTargetObject(GetTargetObject());
		break;
	case COMPONENT_TYPE::PHYSX:
		m_TargetObject->AddComponent(new CPhysX);
		SetTargetObject(GetTargetObject());
		break;
	case COMPONENT_TYPE::LANDSCAPE:
		break;
	default:
		break;
	}
}

void Inspector::SavePrefab()
{
	wstring ContentPath = CPathMgr::GetContentPath();
	wstring FileName	= L"prefab\\";
	FileName += path(m_TargetObject->GetName()).stem();
	FileName += L".pref";

	if (m_bPrefab)
	{
		FileName = path(m_TargetAsset->GetKey()).wstring();
		if (FileName == L"") return;
	}

	filesystem::path file_path = filesystem::path(ContentPath) / FileName;

	if (filesystem::exists(file_path))
	{
		filesystem::remove(file_path);
	}

	CGameObject* pObj = GetTargetObject();
	pObj			  = pObj->Clone();
	wstring Key;
	Key = FileName;

	Ptr<CPrefab> pPrefab = new CPrefab(pObj, false);
	CAssetMgr::GetInst()->AddAsset<CPrefab>(Key, pPrefab.Get());
	pPrefab->Save(Key);

	Content* ContentUI = (Content*)CImGuiMgr::GetInst()->FindUI("##Content");
	ContentUI->ResetBrowser();
	ContentUI->SetTargetDirectory(path(FileName).parent_path().string());
}

void Inspector::DeleteTargetComponent(COMPONENT_TYPE _type)
{
	if (nullptr != m_TargetObject->GetComponent(_type))
	{
		m_TargetObject->DeleteComponent(_type);
	}

	SetTargetObject(GetTargetObject());
}

void Inspector::DeleteTargetScript(ScriptUI* _ScriptUI)
{
	CScript* pScript = _ScriptUI->GetTargetScript();
	m_TargetObject->DeleteScript(pScript);

	SetTargetObject(GetTargetObject());
}

ComponentUI* Inspector::GetComponentUI(COMPONENT_TYPE ComType)
{
	return this->m_arrComUI[(UINT)ComType];
}

void Inspector::ResetComponent()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		ComponentUI* pUI = this->GetComponentUI((COMPONENT_TYPE)i);

		if (pUI != nullptr)
			pUI->ResetUIinfo();
	}
}
