#include "pch.h"
#include "CAfterImage.h"

#include <Engine\CDevice.h>
#include <Engine\CStructuredBuffer.h>

CAfterImage::CAfterImage()
	: CScript((UINT)SCRIPT_TYPE::AFTERIMAGE)
{
	m_info.NodeCount = 1;
}

CAfterImage::~CAfterImage()
{
}

void CAfterImage::begin()
{
}

void CAfterImage::tick()
{
	Vec3 UpdateDir = GetOwner()->Transform()->GetRelativeRotation();

	m_info.NodeRotation[0] = UpdateDir;
}

#include <Engine\CLogMgr.h>
void CAfterImage::UpdateData()
{
	CStructuredBuffer* AfterImageBuffer = CDevice::GetInst()->GetStructuredBuffer(SB_TYPE::AFTERIMAGE);
	AfterImageBuffer->SetData(&m_info, 1);
	AfterImageBuffer->UpdateData(29);
}

void CAfterImage::Clear()
{
	CStructuredBuffer* AfterImageBuffer = CDevice::GetInst()->GetStructuredBuffer(SB_TYPE::AFTERIMAGE);
	AfterImageBuffer->Clear(29);
}
