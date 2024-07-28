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

	// AppendScriptParam("Bloom::Activate", SCRIPT_PARAM::BOOL, &(RENDERMGR->m_BloomInfo.Activate));
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
	auto vecMat = GetOwner()->GetRenderComponent()->GetVecMtrls();
	for (auto& e : vecMat)
	{
		e.pCurMtrl->SetBloom(true);
	}

	m_Buffer->SetData(&m_Info, 1);
	m_Buffer->UpdateData(REGISTER_NUM_BLOOM);
}

void CBloomScript::Clear()
{
	GetOwner()->GetRenderComponent()->GetMaterial(0)->SetBloom(false);
	m_Buffer->Clear(REGISTER_NUM_BLOOM);
}
