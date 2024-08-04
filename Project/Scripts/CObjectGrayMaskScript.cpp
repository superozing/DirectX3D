#include "pch.h"
#include "CObjectGrayMaskScript.h"
#include "Engine\CDevice.h"

CObjectGrayMaskScript::CObjectGrayMaskScript()
	: CScript((UINT)SCRIPT_TYPE::OBJECTGRAYMASKSCRIPT)
{
}

CObjectGrayMaskScript::~CObjectGrayMaskScript()
{
}

void CObjectGrayMaskScript::begin()
{
}

void CObjectGrayMaskScript::tick()
{
}

void CObjectGrayMaskScript::UpdateData()
{
	auto MR = MeshRender();
	if (nullptr == MR)
		return;
	auto Mask = MR->GetStencilMask();
	Mask	  = Mask | (1 << (UINT)STENCIL_TYPE::GRAY);
	MR->SetStencilMask(Mask);
	CONTEXT->OMSetDepthStencilState(CDevice::GetInst()->GetDSState(DS_TYPE::STENCIL_WRITE).Get(), 2);
}

void CObjectGrayMaskScript::Clear()
{
}

void CObjectGrayMaskScript::SaveToFile(ofstream& fout)
{
}

void CObjectGrayMaskScript::LoadFromFile(ifstream& fin)
{
}
