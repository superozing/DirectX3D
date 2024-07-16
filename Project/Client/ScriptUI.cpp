#include "pch.h"
#include "ScriptUI.h"

#include <Scripts/CScriptMgr.h>

#include "ParamUI.h"

ScriptUI::ScriptUI()
	: ComponentUI("Script", "##Script", COMPONENT_TYPE::SCRIPT)
	, m_TargetScript(nullptr)
{
}

ScriptUI::~ScriptUI()
{
}

void ScriptUI::SetScript(CScript* _Script)
{
	m_TargetScript = _Script;

	wstring strScriptName = CScriptMgr::GetScriptName(_Script);
	SetName(ToString(strScriptName));
	SetComponentTitle(ToString(strScriptName));
	Activate();
}

void ScriptUI::render_update()
{
	ComponentUI::render_update();

	auto ScriptList = magic_enum::enum_names<SCRIPT_TYPE>();
	UINT ScriptIdx	= m_TargetScript->GetScriptType();
	if (!TitleCollapse(string(ScriptList[ScriptIdx]).c_str()))
		return;

	const auto& vParam = m_TargetScript->GetScriptParam();

	auto iter = vParam.begin();

	for (; iter != vParam.end(); ++iter)
	{
		switch (iter->second.Type)
		{
		case SCRIPT_PARAM::BOOL:
			ParamUI::Param_BOOL((bool*)iter->second.pData, iter->first, iter->second.View, iter->second.Tooltip);
			break;
		case SCRIPT_PARAM::INT:
			ParamUI::Param_INT((int*)iter->second.pData, iter->first, (int)iter->second.fmin, (int)iter->second.fMax,
							   iter->second.View, iter->second.Tooltip);
			break;
		case SCRIPT_PARAM::FLOAT:
			ParamUI::Param_FLOAT((float*)iter->second.pData, iter->first, iter->second.fmin, iter->second.fMax,
								 iter->second.View, iter->second.Tooltip, iter->second.b_Precision);
			break;
		case SCRIPT_PARAM::STRING:
			ParamUI::Param_STRING((string*)iter->second.pData, iter->first, iter->second.fmin, iter->second.fMax,
								  iter->second.View, iter->second.Tooltip);
			break;
		case SCRIPT_PARAM::VEC2:
			ParamUI::Param_VEC2((Vec2*)iter->second.pData, iter->first, iter->second.fmin, iter->second.fMax,
								iter->second.View, iter->second.Tooltip);
			break;
		case SCRIPT_PARAM::VEC3:
			ParamUI::Param_VEC3((Vec3*)iter->second.pData, iter->first, iter->second.fmin, iter->second.fMax,
								iter->second.View, iter->second.Tooltip);
			break;
		case SCRIPT_PARAM::VEC4:
			ParamUI::Param_VEC4((Vec4*)iter->second.pData, iter->first, iter->second.fmin, iter->second.fMax,
								iter->second.View, iter->second.Tooltip);
			break;
		case SCRIPT_PARAM::OBJECT:
			ParamUI::Param_OBJECT((CGameObject**)iter->second.pData, iter->first, iter->second.CompType,
								  iter->second.ScriptType, iter->second.View, iter->second.Tooltip);
			break;
		case SCRIPT_PARAM::COLOR:
			ParamUI::Param_COLOR((Vec4*)iter->second.pData, iter->first, iter->second.View, iter->second.Tooltip);
			break;
		case SCRIPT_PARAM::FUNC_STATIC:
			ParamUI::Param_FUNC_STATIC(iter->second.StaticFunc, iter->first);
			break;
		case SCRIPT_PARAM::FUNC_MEMBER:
			ParamUI::Param_FUNC_MEMBER(iter->second.MemberFunc, iter->first);
			break;
		case SCRIPT_PARAM::MGR_PHYSX:
			ParamUI::Param_MGR_PHYSX(iter->second.pData);
			break;
		}
	}
}