#pragma once

#include <Engine/CTexture.h>
#include <variant>

typedef void (*StaticFuncPtr)();
class ParamUI
{
private:
	static int g_ID;

public:
	static void ResetID() { g_ID = 0; }
	static bool Param_BOOL(bool* _Data, const string& _Desc, bool _View, const string& _Tooltip);
	static bool Param_INT(int* _Data, const string& _Desc, int _min, int _Max, bool _View, const string& _Tooltip);
	static bool Param_FLOAT(float* _Data, const string& _Desc, float _min, float _Max, bool _View,
							const string& _Tooltip, bool _Precision = false);
	static bool Param_STRING(string* _Data, const string& _Desc, float _min, float _Max, bool _View,
							 const string& _Tooltip);
	static bool Param_VEC2(Vec2* _Data, const string& _Desc, float _min, float _Max, bool _View,
						   const string& _Tooltip);
	static bool Param_VEC3(Vec3* _Data, const string& _Desc, float _min, float _Max, bool _View,
						   const string& _Tooltip);
	static bool Param_VEC4(Vec4* _Data, const string& _Desc, float _min, float _Max, bool _View,
						   const string& _Tooltip);
	static bool Param_COLOR(Vec4* _Data, const string& _Desc, bool _View, const string& _Tooltip = {});
	static bool Param_TEXTURE(_Inout_ Ptr<CTexture>& _Texture, const string& _Desc, UI* _Inst = nullptr,
							  Delegate_1 _Func = nullptr);

	static bool Param_OBJECT(CGameObject** _Object, const string& _Desc, COMPONENT_TYPE _Ctype, UINT _Stype, bool _View,
							 const string& _Tooltip);

	static bool Param_Asset(CAsset** _Asset, const string& _Desc, ASSET_TYPE _Type, bool _View, const string& _Tooltip);

	static bool Param_FUNC_STATIC(StaticFuncPtr _Func, const string& _Desc);
	static bool Param_FUNC_MEMBER(std::function<void()> _Func, const string& _Desc);

	static bool Param_MGR_PHYSX(void* _pPhysXMgr);
	static void Param_Line();
	static void Param_SameLine();

	template <typename T>
	static bool Param_Vector(vector<T>* _vec, const string& _Desc, int* _idx, bool _View, const string& _Tooltip);
};

template <typename T>
inline bool ParamUI::Param_Vector(vector<T>* _vec, const string& _Desc, int* _idx, bool _View, const string& _Tooltip)
{
	ImGui::Text(_Desc.c_str());

	if (_vec->size() <= 0)
		return false;
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##paramvec%d", g_ID++);

	char szCombID[256] = {};
	sprintf_s(szID, "##Combo%d", g_ID++);

	int idx = *_idx;

	idx = clamp(idx, 0, (int)_vec->size() - 1);

	vector<T> vec = *_vec;
	string	  str = vec[idx];

	if (ImGui::BeginCombo(szID, str.c_str()))
	{
		for (int n = 0; n < _vec->size(); n++)
		{
			bool is_selected = (str == vec[n]);
			if (ImGui::Selectable(vec[n].c_str(), is_selected))
			{
				str = vec[n];

				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
				*_idx = n;
			}
		}
		ImGui::EndCombo();
	}

	return false;
}
