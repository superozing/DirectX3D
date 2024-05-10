#include "pch.h"
#include "CRenderMgrScript.h"

#include <Engine/CRenderMgr.h>

#define RENDERMGR CRenderMgr::GetInst()

LerpFloatFuncPtr lerpFloatFunc = RoRMath::Lerp;
TestPram CRenderMgrScript::TestParam = {};

CRenderMgrScript::CRenderMgrScript()
	: CScript((UINT)SCRIPT_TYPE::RENDERMGRSCRIPT)
{
	AppendStaticFunction("LerpFloat", SCRIPT_PARAM::FUNC_STATIC, "Lerp", &CRenderMgrScript::LerpTest);

	AppendScriptParam("From", SCRIPT_PARAM::FLOAT, &TestParam.T1);
	AppendScriptParam("To", SCRIPT_PARAM::FLOAT, &TestParam.T2);
	AppendScriptParam("Alpha", SCRIPT_PARAM::FLOAT, &TestParam.T3);
	AppendScriptParam("Res", SCRIPT_PARAM::FLOAT, &TestParam.result);

	m_TestParam = {1.f,2.f,3.f,4.f};

	AppendScriptParam("m_From", SCRIPT_PARAM::FLOAT, &m_TestParam.T1);
	AppendScriptParam("m_To", SCRIPT_PARAM::FLOAT, &m_TestParam.T2);
	AppendScriptParam("m_Alpha", SCRIPT_PARAM::FLOAT, &m_TestParam.T3);
	AppendScriptParam("m_Res", SCRIPT_PARAM::FLOAT, &m_TestParam.result);
	AppendMemberFunction("m_LerpFloat", SCRIPT_PARAM::FUNC_MEMBER, "Lerp", std::bind(&CRenderMgrScript::m_LerpTest, this));
}

CRenderMgrScript::~CRenderMgrScript()
{
}


void CRenderMgrScript::begin()
{
	AppendScriptParam("Clear Color", SCRIPT_PARAM::COLOR, &(RENDERMGR->m_vClearColor));
}

void CRenderMgrScript::tick()
{
}

void CRenderMgrScript::SaveToFile(FILE* _File)
{
}

void CRenderMgrScript::LoadFromFile(FILE* _File)
{
}

