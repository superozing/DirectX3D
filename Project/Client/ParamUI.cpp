#include "pch.h"
#include "ParamUI.h"

#include <Engine/CAssetMgr.h>

#include "imgui.h"
#include "ListUI.h"
#include <Engine/CPhysXMgr.h>

int ParamUI::g_ID = 0;

bool ParamUI::Param_BOOL(bool* _Data, const string& _Desc, bool _View, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##bool%d", g_ID++);

	if (_View)
	{
		ImGui::BeginDisabled();

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::Checkbox(szID, _Data))
		{
			return true;
		}

		ImGui::EndDisabled();
	}
	else
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::Checkbox(szID, _Data))
		{
			return true;
		}
	}

	return false;
}

bool ParamUI::Param_INT(int* _Data, const string& _Desc, int _min, int _Max, bool _View, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##int%d", g_ID++);

	if (_View)
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::DragInt(szID, _Data, 0.f, 0, 0, "%d", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput))
		{
			return true;
		}
	}
	else
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::DragInt(szID, _Data, 1.f, _min, _Max))
		{
			return true;
		}
	}

	return false;
}

bool ParamUI::Param_FLOAT(float* _Data, const string& _Desc, float _min, float _Max, bool _View, const string& _Tooltip,
						  bool _Precision)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##float%d", g_ID++);

	if (_View)
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::DragFloat(szID, _Data, 0.f, 0.f, 0.f, (true == _Precision) ? "%.2f" : "%.1f",
							 ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput))
		{
			return true;
		}
	}
	else
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::DragFloat(szID, _Data, (true == _Precision) ? 0.01f : 0.1f, _min, _Max,
							 (true == _Precision) ? "%.2f" : "%.1f"))
		{
			return true;
		}
	}

	return false;
}

bool ParamUI::Param_STRING(string* _Data, const string& _Desc, float _min, float _Max, bool _View,
						   const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	if (!_Data)
		return false;
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##float%d", g_ID++);

	if (_View)
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		char buffer[255];
		strcpy_s(buffer, _Data->c_str());
		ImGui::InputText(szID, buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_ReadOnly);
	}
	else
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		char buffer[255];
		strcpy_s(buffer, _Data->c_str());
		if (ImGui::InputText(szID, buffer, IM_ARRAYSIZE(buffer)))
		{
			(*_Data) = buffer;
			return true;
		}
	}

	return false;
}

bool ParamUI::Param_VEC2(Vec2* _Data, const string& _Desc, float _min, float _Max, bool _View, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	float arrFloat[2] = {_Data->x, _Data->y};
	char  szID[256]	  = {};
	sprintf_s(szID, "##Vec2%d", g_ID++);

	if (_View)
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::DragFloat2(szID, arrFloat, 0.f, 0.f, 0.f, "%.1f",
							  ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput))
		{
			_Data->x = arrFloat[0];
			_Data->y = arrFloat[1];
			return true;
		}
	}
	else
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::DragFloat2(szID, arrFloat, 0.1f, _min, _Max, "%.1f"))
		{
			_Data->x = arrFloat[0];
			_Data->y = arrFloat[1];
			return true;
		}
	}

	return false;
}

bool ParamUI::Param_VEC3(Vec3* _Data, const string& _Desc, float _min, float _Max, bool _View, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##Vec4%d", g_ID++);

	if (_View)
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::DragFloat3(szID, *_Data, 0.f, 0.f, 0.f, "%.1f",
							  ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput))
		{
			return true;
		}
	}
	else
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::DragFloat3(szID, *_Data, 0.1f, _min, _Max, "%.1f"))
		{
			return true;
		}
	}

	return false;
}

bool ParamUI::Param_VEC4(Vec4* _Data, const string& _Desc, float _min, float _Max, bool _View, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##Vec4%d", g_ID++);

	if (_View)
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::DragFloat4(szID, *_Data, 0.f, 0.f, 0.f, "%.1f",
							  ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput))
		{
			return true;
		}
	}
	else
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::DragFloat4(szID, *_Data, 0.1f, _min, _Max, "%.1f"))
		{
			return true;
		}
	}

	return false;
}

bool ParamUI::Param_COLOR(Vec4* _Data, const string& _Desc, bool _View, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##Vec4%d", g_ID++);

	if (_View)
	{
		ImGui::BeginDisabled();

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		UI::ColorSelector(szID, _Data);

		ImGui::EndDisabled();

		return true;
	}
	else
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		UI::ColorSelector(szID, _Data);
	}

	return false;
}

bool ParamUI::Param_TEXTURE(Ptr<CTexture>& _Texture, const string& _Desc, UI* _Inst, Delegate_1 _Func)
{
	// 파라미터의 Description 정보 출력
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	// Texture 이름 출력 InputText 의 ID 만들기
	char szID[256] = {};
	sprintf_s(szID, "##Texture%d", g_ID++);

	ImTextureID texid = nullptr;
	string		strTexKey;
	if (nullptr != _Texture)
	{
		texid	  = _Texture->GetSRV().Get();
		strTexKey = string(_Texture->GetKey().begin(), _Texture->GetKey().end());
	}

	ImGui::InputText(szID, (char*)strTexKey.c_str(), strTexKey.length(), ImGuiInputTextFlags_ReadOnly);

	// Texture Drop 체크
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			DWORD_PTR data	 = *((DWORD_PTR*)payload->Data);
			CAsset*	  pAsset = (CAsset*)data;
			if (ASSET_TYPE::TEXTURE == pAsset->GetType())
			{
				_Texture = (CTexture*)pAsset;
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	string key = "X##";
	key += szID;
	if (ImGui::Button(key.c_str()))
	{
		_Texture = nullptr;
		texid	 = nullptr;
	}

	// 텍스쳐 이미지 출력
	static bool use_text_color_for_tint = false;
	ImVec2		uv_min					= ImVec2(0.0f, 0.0f);
	ImVec2		uv_max					= ImVec2(1.0f, 1.0f);
	ImVec4		tint_col =
		 use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	ImGui::Image(texid, ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
	// Texture Drop 체크
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			DWORD_PTR data	 = *((DWORD_PTR*)payload->Data);
			CAsset*	  pAsset = (CAsset*)data;
			if (ASSET_TYPE::TEXTURE == pAsset->GetType())
			{
				_Texture = (CTexture*)pAsset;
			}
		}

		ImGui::EndDragDropTarget();
	}

	// 입력된 델리게이트가 있다면
	if (_Inst && _Func)
	{
		ImGui::SameLine();
		sprintf_s(szID, "##TexBtn%d", g_ID++);
		if (ImGui::Button(szID, ImVec2(20, 20)))
		{
			// 리스트 UI
			ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##List");

			vector<string> vecTexName;
			CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::TEXTURE, vecTexName);

			pListUI->AddString(vecTexName);
			pListUI->SetDbClickDelegate(_Inst, (Delegate_1)_Func);
			pListUI->Activate();

			return true;
		}
	}

	return false;
}

#include <Engine/CGameObject.h>
#include <Engine/CScript.h>

bool ParamUI::Param_OBJECT(CGameObject** _Object, const string& _Desc, COMPONENT_TYPE _Ctype, UINT _Stype, bool _View,
						   const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##obj%d", g_ID++);

	string name = "";
	if (*_Object)
		name = ToString((*_Object)->GetName());

	if (_View)
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::InputText(szID, (char*)name.c_str(), name.length(), ImGuiInputTextFlags_ReadOnly))
		{
			return true;
		}
	}
	else
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::InputText(szID, (char*)name.c_str(), name.length(), ImGuiInputTextFlags_ReadOnly))
		{
			return true;
		}
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OutlinerTree");

			if (payload)
			{
				DWORD_PTR data	  = *((DWORD_PTR*)payload->Data);
				CEntity*  ptrData = (CEntity*)data;

				CGameObject* obj = dynamic_cast<CGameObject*>(ptrData);

				if (obj && (_Ctype != COMPONENT_TYPE::END && obj->GetComponent(_Ctype)))
				{
					*_Object = obj;
				}

				if (obj)
				{
					auto scripts = obj->GetScripts();
					bool find	 = false;
					for (auto script : scripts)
					{
						UINT type = script->GetScriptType();
						if (type == _Stype)
						{
							*_Object = obj;
						}
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

	return false;
}

bool ParamUI::Param_Asset(CAsset** _Asset, const string& _Desc, ASSET_TYPE _Type, bool _View, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##asset%d", g_ID++);

	string name = "";
	if (*_Asset)
		name = ToString((*_Asset)->GetKey());

	if (_View)
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::InputText(szID, (char*)name.c_str(), name.length(), ImGuiInputTextFlags_ReadOnly))
		{
			return true;
		}
	}
	else
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::InputText(szID, (char*)name.c_str(), name.length(), ImGuiInputTextFlags_ReadOnly))
		{
			return true;
		}
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

			if (payload)
			{
				DWORD_PTR data	  = *((DWORD_PTR*)payload->Data);
				CEntity*  ptrData = (CEntity*)data;

				CAsset* asset = dynamic_cast<CAsset*>(ptrData);

				if (asset && _Type == asset->GetType())
				{
					*_Asset = asset;
				}
			}
			ImGui::EndDragDropTarget();
			return true;
		}
	}

	return false;
}

bool ParamUI::Param_FUNC_STATIC(StaticFuncPtr _Func, const string& _Desc)
{
	if (ImGui::Button(_Desc.c_str()))
	{
		_Func();
		return true;
	}
	return false;
}

bool ParamUI::Param_FUNC_MEMBER(std::function<void()> _Func, const string& _Desc)
{
	if (ImGui::Button(_Desc.c_str()))
	{
		_Func();
		return true;
	}
	return false;
}

bool ParamUI::Param_MGR_PHYSX(void* _pPhysXMgr)
{
	CPhysXMgr* pPhysXMgr = static_cast<CPhysXMgr*>(_pPhysXMgr);
	if (nullptr == pPhysXMgr)
	{
		return false;
	}

	// ImGui::DragFloat("GravityMul:", &(pPhysXMgr->m_fGravityMul));
	// ImGui::DragFloat("LestOffset:", &(pPhysXMgr->m_fLestOffset));

	// 레이어 수를 정의합니다.
	static float InvTimeStep;
	InvTimeStep = 1.f / pPhysXMgr->m_TimeStep;
	ImGui::DragFloat("TimeStep(FPS):", &(InvTimeStep));
	pPhysXMgr->m_TimeStep		 = 1.f / InvTimeStep;
	const int		  LayerCount = static_cast<int>(LAYER::LAYER_MAX);
	static const auto LayerMap	 = GamePlayStatic::GetLayerMap();

	ImGui::Checkbox("UseTransformTreshold", &(pPhysXMgr->m_bUseTH));
	ImGui::DragFloat2("PosTreshold", &(pPhysXMgr->m_fPosTreshold.x), 0.0001f, 0.f, 10.f, "%.4f");
	ImGui::DragFloat2("RotTreshold", &(pPhysXMgr->m_fRotTreshold.x), 0.0001f, 0.f, 10.f, "%.4f");

	static ImGuiTableFlags table_flags = ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerH |
										 ImGuiTableFlags_Hideable | ImGuiTableFlags_Resizable |
										 ImGuiTableFlags_HighlightHoveredColumn;
	static bool bools[LayerCount * LayerCount] = {}; // Dummy storage selection storage
	static int	frozen_cols					   = 1;
	static int	frozen_rows					   = 2;

	if (ImGui::CollapsingHeader("Layer Table"))
	{
		if (ImGui::BeginTable("table_angled_headers", LayerMap.size() + 1, table_flags,
							  ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing())))
		{
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
			for (int n = 0; n < LayerMap.size(); n++)
				ImGui::TableSetupColumn(LayerMap[n].LayerName.c_str(),
										ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupScrollFreeze(frozen_cols, frozen_rows);

			ImGui::TableAngledHeadersRow(); // Draw angled headers for all columns with the
											// ImGuiTableColumnFlags_AngledHeader flag.
			ImGui::TableHeadersRow();		// Draw remaining headers and allow access to context-menu and other
											// functions.
			for (int row = 0; row < LayerMap.size(); row++)
			{
				ImGui::PushID(row);
				// ImGui::TableNextRow(ImGuiTableRowFlags_None, 0.f); // 행 높이를 설정
				ImGui::TableNextRow(); // 행 높이를 설정
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::Text(LayerMap[row].LayerName.c_str(), row);
				for (int column = 1; column <= LayerMap.size(); column++)
					if (ImGui::TableSetColumnIndex(column))
					{
						ImGui::PushID(column);
						int realrow = LayerMap[row].LayerIdx;
						int realcol = LayerMap[column - 1].LayerIdx;
						// 현재 상태를 가져옵니다.
						bool isColliding = (CPhysXMgr::m_layerMasks[realrow] & (1 << realcol)) != 0;
						if (realrow <= realcol)
						{
							// 체크박스를 그립니다.
							if (ImGui::Checkbox("", &isColliding))
							{
								// 체크 상태가 변경되면 충돌 정보를 업데이트합니다.
								pPhysXMgr->LayerCheckToggle((UINT)realrow, (UINT)realcol);
							}
						}
						else
						{
							// i == j 인 경우 체크박스를 비활성화하고 상태만 표시합니다.
							ImGui::BeginDisabled();
							ImGui::Checkbox("", &isColliding);
							ImGui::EndDisabled();
						}
						ImGui::PopID();
					}
				ImGui::PopID();
			}
			ImGui::EndTable();
		}
	}

	for (const auto& info : pPhysXMgr->m_vecTriggerColInfo)
	{
		auto ob1 = static_cast<CGameObject*>(info.Actor1->userData);
		auto ob2 = static_cast<CGameObject*>(info.Actor2->userData);
		auto msg = "" + ToString(ob1->GetName()) + " + " + ToString(ob2->GetName());
		ImGui::Text(msg.c_str());
	}
	return true;
}

void ParamUI::Param_Line()
{
	ImGui::Separator();
}

void ParamUI::Param_SameLine()
{
	ImGui::SameLine();
}
