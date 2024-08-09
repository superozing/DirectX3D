#include "pch.h"
#include "CRenderMgrScript.h"

#include <Engine/CRenderMgr.h>

#define RENDERMGR CRenderMgr::GetInst()

LerpFloatFuncPtr lerpFloatFunc				 = RoRMath::Lerp;
TestPram		 CRenderMgrScript::TestParam = {};

CRenderMgrScript::CRenderMgrScript()
	: CScript((UINT)SCRIPT_TYPE::RENDERMGRSCRIPT)
{
	// AppendStaticFunction("LerpFloat", SCRIPT_PARAM::FUNC_STATIC, "Lerp", &CRenderMgrScript::LerpTest);

	// AppendScriptParam("From", SCRIPT_PARAM::FLOAT, &TestParam.T1);
	// AppendScriptParam("To", SCRIPT_PARAM::FLOAT, &TestParam.T2);
	// AppendScriptParam("Alpha", SCRIPT_PARAM::FLOAT, &TestParam.T3);
	// AppendScriptParam("Res", SCRIPT_PARAM::FLOAT, &TestParam.result);

	// m_TestParam = {1.f, 2.f, 3.f, 4.f};

	// AppendScriptParam("m_From", SCRIPT_PARAM::FLOAT, &m_TestParam.T1);
	// AppendScriptParam("m_To", SCRIPT_PARAM::FLOAT, &m_TestParam.T2);
	// AppendScriptParam("m_Alpha", SCRIPT_PARAM::FLOAT, &m_TestParam.T3);
	// AppendScriptParam("m_Res", SCRIPT_PARAM::FLOAT, &m_TestParam.result);
	// AppendMemberFunction("m_LerpFloat", SCRIPT_PARAM::FUNC_MEMBER, "Lerp",
	//					 std::bind(&CRenderMgrScript::m_LerpTest, this));

	// ClearColor
	AppendScriptParam("Clear Color", SCRIPT_PARAM::COLOR, &(RENDERMGR->m_vClearColor));

	// Bloom
	AppendScriptParam("Bloom::Activate", SCRIPT_PARAM::BOOL, &(RENDERMGR->m_GlobalBloomInfo.BloomActivate));
	AppendScriptParam("Bloom::Global Bloom", SCRIPT_PARAM::BOOL, &(RENDERMGR->m_GlobalBloomInfo.GlobalBloom));
	AppendScriptParam("Bloom::Global Threshold", SCRIPT_PARAM::FLOAT,
					  &(RENDERMGR->m_GlobalBloomInfo.GlbalBloomSetting.fThreshold), 0.f, 1.f, false, "", true);
	AppendScriptParam("Bloom::Global Color", SCRIPT_PARAM::COLOR,
					  &(RENDERMGR->m_GlobalBloomInfo.GlbalBloomSetting.vBloomColor));

	AppendScriptParam("Bloom::BlurCnt", SCRIPT_PARAM::INT, &(RENDERMGR->m_GlobalBloomInfo.BlurLevel), 0, MAXBLURLEVEL);
	AppendScriptParam("Bloom::MergeRatio", SCRIPT_PARAM::FLOAT, &(RENDERMGR->m_GlobalBloomInfo.Ratio), 0.f, 0.f, false,
					  "", true);

	// CromaticAberration
	AppendMemberFunction("CA::PushEvent", SCRIPT_PARAM::FUNC_MEMBER, "PushEvent",
						 std::bind(&CRenderMgr::PushCAEvent, RENDERMGR));
	AppendScriptParam("CA::Activate", SCRIPT_PARAM::BOOL, &(RENDERMGR->m_CAInfo.Activate));
	AppendScriptParam("CA::Duration", SCRIPT_PARAM::FLOAT, &(RENDERMGR->m_CAInfo.Duration));
	AppendScriptParam("CA::RedOffset", SCRIPT_PARAM::VEC2, &(RENDERMGR->m_CAInfo.MaxRedOffSet));
	AppendScriptParam("CA::GreenOffset", SCRIPT_PARAM::VEC2, &(RENDERMGR->m_CAInfo.MaxGreenOffset));
	AppendScriptParam("CA::BlueOffset", SCRIPT_PARAM::VEC2, &(RENDERMGR->m_CAInfo.MaxBlueOffset));
	AppendScriptParam("CA::CropOffset", SCRIPT_PARAM::VEC2, &(RENDERMGR->m_CAInfo.CropOffset));

	// Vignette
	AppendMemberFunction("Vignette::Activate", SCRIPT_PARAM::FUNC_MEMBER, "Switch Vignette",
						 std::bind(&CRenderMgr::SwitchVignette, RENDERMGR));

	AppendScriptParam("Vignette Active", SCRIPT_PARAM::BOOL, &(RENDERMGR->m_VignetteInfo.bVignetteRender));
	AppendScriptParam("Vignette Power", SCRIPT_PARAM::INT, &(RENDERMGR->m_VignetteInfo.iVignettePower));

	AppendScriptParam("GRAY::bGrayWeight", SCRIPT_PARAM::BOOL, &(RENDERMGR->m_bGrayWeight));
}

CRenderMgrScript::~CRenderMgrScript()
{
}

void CRenderMgrScript::begin()
{
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
