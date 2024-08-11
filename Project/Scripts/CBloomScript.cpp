#include "pch.h"
#include "CBloomScript.h"

#include <Engine\CDevice.h>
#include <Engine\CStructuredBuffer.h>
#include <Engine\CRenderMgr.h>
#define REGISTER_NUM_BLOOM 15
#define RENDERMGR CRenderMgr::GetInst()

CBloomScript::CBloomScript()
	: CScript((UINT)SCRIPT_TYPE::BLOOMSCRIPT)
{
	m_Buffer		   = CDevice::GetInst()->GetStructuredBuffer(SB_TYPE::BLOOM);
	m_Info.vBloomColor = Vec4(1.f, 1.f, 1.f, 1.f);
	m_Info.fThreshold  = 0.8f;

	AppendScriptParam("Bloom Activate", SCRIPT_PARAM::BOOL, &(m_bBloomActive));
	AppendScriptParam("Bloom Original Color Use", SCRIPT_PARAM::BOOL, &(m_Info.bUseOriginalColor));
	AppendScriptParam("Bloom Color", SCRIPT_PARAM::COLOR, &(m_Info.vBloomColor));
	AppendScriptParam("Bloom Threshold", SCRIPT_PARAM::FLOAT, &(m_Info.fThreshold), 0.f, 1.f, false, "", true);
}

CBloomScript::CBloomScript(const CBloomScript&)
	: CScript((UINT)SCRIPT_TYPE::BLOOMSCRIPT)
{
	m_Buffer		   = CDevice::GetInst()->GetStructuredBuffer(SB_TYPE::BLOOM);
	m_Info.vBloomColor = Vec4(1.f, 1.f, 1.f, 1.f);
	m_Info.fThreshold  = 0.8f;

	AppendScriptParam("Bloom Activate", SCRIPT_PARAM::BOOL, &(m_bBloomActive));
	AppendScriptParam("Bloom Color", SCRIPT_PARAM::COLOR, &(m_Info.vBloomColor));
	AppendScriptParam("Bloom Threshold", SCRIPT_PARAM::FLOAT, &(m_Info.fThreshold), 0.f, 1.f, false, "", true);
}

CBloomScript::~CBloomScript()
{
}

void CBloomScript::begin()
{
}

void CBloomScript::tick()
{
}

void CBloomScript::UpdateData()
{
	// 블룸기능이 꺼져있으면
	if (false == RENDERMGR->m_GlobalBloomInfo.BloomActivate)
		return;
	// 개인블룸과 글로벌블룸이 모두 꺼져있는 경우
	if (false == m_bBloomActive && false == RENDERMGR->m_GlobalBloomInfo.GlobalBloom)
		return;

	auto ObjName = GetOwner()->GetName();

	auto vecMat = GetOwner()->GetRenderComponent()->GetVecMtrls();
	for (auto& e : vecMat)
	{
		e.pCurMtrl->SetBloom(true);
	}

	// 오브젝트별 블룸
	if (true == m_bBloomActive)
	{
		m_Buffer->SetData(&m_Info, 1);
	}
	// 전역 블룸
	else
	{
		m_Buffer->SetData(&(RENDERMGR->m_GlobalBloomInfo.GlbalBloomSetting), 1);
	}
	m_Buffer->UpdateData(REGISTER_NUM_BLOOM);
}

void CBloomScript::Clear()
{
	GetOwner()->GetRenderComponent()->GetMaterial(0)->SetBloom(false);
	m_Buffer->Clear(REGISTER_NUM_BLOOM);
}

#define TagBloomActivate "[BloomActivate]"
#define TagBloomColor "[BloomColor]"
#define TagBloomThreshold "[BloomThreshold]"
#define TagBloombUseOriginalColor "[BloomOriginalColor]"
void CBloomScript::SaveToFile(ofstream& fout)
{
	fout << TagBloomActivate << endl;
	fout << m_bBloomActive << endl;

	fout << TagBloomColor << endl;
	fout << m_Info.vBloomColor << endl;

	fout << TagBloomThreshold << endl;
	fout << m_Info.fThreshold << endl;

	fout << TagBloombUseOriginalColor << endl;
	fout << m_Info.bUseOriginalColor << endl;
}

void CBloomScript::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagBloomActivate);
	fin >> m_bBloomActive;

	Utils::GetLineUntilString(fin, TagBloomColor);
	fin >> m_Info.vBloomColor;

	Utils::GetLineUntilString(fin, TagBloomThreshold);
	fin >> m_Info.fThreshold;

	Utils::GetLineUntilString(fin, TagBloombUseOriginalColor);
	fin >> m_Info.bUseOriginalColor;
}
