#include "pch.h"
#include "Inspector.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine/components.h>

#include "CImGuiMgr.h"
#include "Outliner.h"

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

Inspector::Inspector(CGameObject* _Prefab)
	: UI("PrefabInspector", "##PrefabInspector")
	, m_TargetObject(nullptr)
	, m_arrComUI{}
{
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
				wstring ContentPath = CPathMgr::GetContentPath();
				ContentPath += L"prefab\\";

				wstring FileName = m_TargetObject->GetName();
				FileName += L".pref";

				SavePrefab(ToString(ContentPath), ToString(FileName));
			}

			ObjectLayer();
		}
		else
		{
			ImGui::SameLine();
			if (ImGui::Button("Save Prefab"))
			{
				wstring ContentPath = CPathMgr::GetContentPath();
				ContentPath += L"prefab\\";

				wstring FileName = m_TargetObject->GetName();
				FileName += L".pref";

				SavePrefab(ToString(ContentPath), ToString(FileName));
			}

			int LayerIdx = PrefabLayer(); ImGui::SameLine();

			if (ImGui::Button("Spawn Prefab"))
			{
				Vec3 vPos = m_TargetObject->Transform()->GetRelativePos();
				m_TargetObject->Transform()->SetRelativePos(vPos);
				GamePlayStatic::SpawnGameObject(m_TargetObject, LayerIdx);
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
	}
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
		
	if(nullptr != m_TargetAsset)
	{
		m_arrAssetUI[(UINT)m_TargetAsset->GetType()]->Activate();
		m_arrAssetUI[(UINT)m_TargetAsset->GetType()]->SetAsset(_Asset);
	}	
}


void Inspector::ObjectName()
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

int Inspector::PrefabLayer()
{
	static int LayerIdx = 0;
	static int PrevIdx = LayerIdx;

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

	return LayerIdx;
}

void Inspector::ObjectComponent()
{
	auto ComponentList = magic_enum::enum_names<COMPONENT_TYPE>();
	
	for (size_t i = 0; i < ComponentList.size() - 2; ++i)
	{
		if (ImGui::MenuItem(string(ComponentList[i]).c_str()))
		{
			CheckComponent((COMPONENT_TYPE)i);
		}
	}
}


void Inspector::CheckComponent(COMPONENT_TYPE _type)
{
	if (nullptr != m_TargetObject->GetComponent((COMPONENT_TYPE)_type))
	{
		MessageBoxA(nullptr, "Already contains the same component", "Can't add the same component multiple times!", MB_OK);
		return;
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
	case COMPONENT_TYPE::LANDSCAPE:
		break;
	default:
		break;
	}
}

void Inspector::MakePrefab()
{
	CGameObject* pObj = GetTargetObject();
	pObj = pObj->Clone();
	wstring Key;
	Key = L"prefab\\" + m_TargetObject->GetName() + L".pref";
	Ptr<CPrefab> pPrefab = new CPrefab(pObj, false);
	CAssetMgr::GetInst()->AddAsset<CPrefab>(Key, pPrefab.Get());
	pPrefab->Save(Key);
}

void Inspector::SavePrefab(const string& _Directory, const string& _FileName)
{
	filesystem::path file_path = filesystem::path(_Directory) / _FileName;

	if (filesystem::exists(file_path))
	{
		filesystem::remove(file_path);
	}

	MakePrefab();
}
