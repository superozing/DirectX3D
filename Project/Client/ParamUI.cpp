#include "pch.h"
#include "ParamUI.h"

#include <Engine/CAssetMgr.h>

#include "imgui.h"
#include "ListUI.h"

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

bool ParamUI::Param_FLOAT(float* _Data, const string& _Desc, float _min, float _Max, bool _View, const string& _Tooltip)
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
		if (ImGui::DragFloat(szID, _Data, 0.f, 0.f, 0.f, "%.1f",
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
		if (ImGui::DragFloat(szID, _Data, 0.1f, _min, _Max, "%.1f"))
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
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##float%d", g_ID++);

	if (_View)
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		ImGui::Text(_Data->c_str());
	}
	else
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		char buffer[255];
		strcpy_s(buffer, _Data->c_str());
		if (ImGui::InputText(_Desc.c_str(), buffer, IM_ARRAYSIZE(buffer)))
		{
			(*_Data) = buffer;
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

	// 텍스쳐 이미지 출력
	static bool use_text_color_for_tint = false;
	ImVec2		uv_min					= ImVec2(0.0f, 0.0f);
	ImVec2		uv_max					= ImVec2(1.0f, 1.0f);
	ImVec4		tint_col =
		 use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	ImGui::Image(texid, ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);

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

// bool ParamUI::Param_OBJECT(CGameObject* _Object, const string& _Desc, COMPONENT_TYPE _Ctype, SCRIPT_TYPE _Stype,
//						   bool _View, const string& _Tooltip)
//{
//	ImGui::Text(_Desc.c_str());
//	ImGui::SameLine();
//
//	char szID[256] = {};
//	sprintf_s(szID, "##obj%d", g_ID++);
//
//	if (_View)
//	{
//		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
//		{
//			ImGui::SetTooltip(_Tooltip.c_str());
//		}
//		if (ImGui::InputText(szID, (char*)_Object->GetName().c_str(), _Object->GetName().length(),
//							 ImGuiInputTextFlags_ReadOnly))
//		{
//			return true;
//		}
//	}
//	else
//	{
//		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
//		{
//			ImGui::SetTooltip(_Tooltip.c_str());
//		}
//		if (ImGui::InputText(szID, (char*)_Object->GetName().c_str(), _Object->GetName().length(),
//							 ImGuiInputTextFlags_ReadOnly))
//		{
//			return true;
//		}
//		if (ImGui::BeginDragDropTarget())
//		{
//			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
//
//			if (payload)
//			{
//				DWORD_PTR data	  = *((DWORD_PTR*)payload->Data);
//				CEntity*  ptrData = (CEntity*)data;
//
//				//	// 레벨 들어가면 터짐
//				//	T* Res = dynamic_cast<T*>(ptrData);
//
//				//	if (Res != nullptr)
//				//	{
//				//		*_Ptr = Res;
//				//		ImGui::EndDragDropTarget();
//				//		return true;
//				//	}
//			}
//
//			ImGui::EndDragDropTarget();
//		}
//	}
//
//	return false;
// }

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
