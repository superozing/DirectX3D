#pragma once

#include <Engine/CTexture.h>
#include <variant>

typedef void (*StaticFuncPtr)();
class ParamUI
{
private:
	static int	g_ID;

public:
	static void ResetID() { g_ID = 0; }
	static bool Param_BOOL(bool* _Data, const string& _Desc, bool _View, const string& _Tooltip);
	static bool Param_INT(int* _Data, const string& _Desc, int _min, int _Max, bool _View, const string& _Tooltip);
	static bool Param_FLOAT(float* _Data, const string& _Desc, float _min, float _Max, bool _View, const string& _Tooltip);
	static bool Param_VEC2(Vec2* _Data, const string& _Desc, float _min, float _Max, bool _View, const string& _Tooltip);
	static bool Param_VEC3(Vec3* _Data, const string& _Desc, float _min, float _Max, bool _View, const string& _Tooltip);
	static bool Param_VEC4(Vec4* _Data, const string& _Desc, float _min, float _Max, bool _View, const string& _Tooltip);
	static bool Param_COLOR(Vec4* _Data, const string& _Desc, bool _View, const string& _Tooltip = {});
	static bool Param_TEXTURE(_Inout_ Ptr<CTexture>& _Texture, const string& _Desc, UI* _Inst = nullptr, Delegate_1 _Func = nullptr);

	static bool Param_FUNC_STATIC(StaticFuncPtr _Func, const string& _Desc);
	static bool Param_FUNC_MEMBER(std::function<void()> _Func, const string& _Desc);
};
