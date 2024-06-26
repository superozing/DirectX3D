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
		if (ImGui::DragFloat(szID, _Data, 0.f, 0.f, 0.f, "%.1f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput))
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

bool ParamUI::Param_VEC2(Vec2* _Data, const string& _Desc, float _min, float _Max, bool _View, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	float arrFloat[2] = { _Data->x, _Data->y };
	char szID[256] = {};
	sprintf_s(szID, "##Vec2%d", g_ID++);

	if (_View)
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !_Tooltip.empty())
		{
			ImGui::SetTooltip(_Tooltip.c_str());
		}
		if (ImGui::DragFloat2(szID, arrFloat, 0.f, 0.f, 0.f, "%.1f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput))
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
		if (ImGui::DragFloat3(szID, *_Data, 0.f, 0.f, 0.f, "%.1f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput))
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
		if (ImGui::DragFloat4(szID, *_Data, 0.f, 0.f, 0.f, "%.1f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput))
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
	string strTexKey;
	if (nullptr != _Texture)
	{
		texid = _Texture->GetSRV().Get();
		strTexKey = string(_Texture->GetKey().begin(), _Texture->GetKey().end());
	}

	ImGui::InputText(szID, (char*)strTexKey.c_str(), strTexKey.length(), ImGuiInputTextFlags_ReadOnly);

	// Texture Drop 체크
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			DWORD_PTR data = *((DWORD_PTR*)payload->Data);
			CAsset* pAsset = (CAsset*)data;
			if (ASSET_TYPE::TEXTURE == pAsset->GetType())
			{
				_Texture = (CTexture*)pAsset;
			}
		}

		ImGui::EndDragDropTarget();
	}


	// 텍스쳐 이미지 출력
	static bool use_text_color_for_tint = false;
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);
	ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
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

	// 레이어 수를 정의합니다.
	const int layerCount = static_cast<int>(LAYER::LAYER_MAX);

	// 충돌 행렬을 N x N 체크박스로 표시합니다.
	for (int i = 0; i < layerCount; ++i)
	{
		for (int j = 0; j < layerCount; ++j)
		{
			// 체크박스의 ID를 고유하게 설정합니다.
			char buf[32];
			_snprintf_s(buf, sizeof(buf), "##%d_%d", i, j);

			// 현재 상태를 가져옵니다.
			bool isColliding = (CPhysXMgr::m_layerMasks[i] & (1 << j)) != 0;

			// 체크박스를 그립니다.
			if (ImGui::Checkbox(buf, &isColliding))
			{
				// 체크 상태가 변경되면 충돌 정보를 업데이트합니다.
				pPhysXMgr->LayerCheckToggle((UINT)i, (UINT)j);
			}

			// 적절한 간격을 추가합니다.
			if (j < layerCount - 1)
				ImGui::SameLine();
		}
		// 다음 행으로 이동합니다.
		ImGui::NewLine();
	}

	return true;
}
