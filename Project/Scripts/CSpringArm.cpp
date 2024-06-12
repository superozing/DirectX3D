#include "pch.h"
#include "CSpringArm.h"

#include <Engine\CRenderMgr.h>

CSpringArm::CSpringArm()
	:CScript((UINT)SCRIPT_TYPE::SPRINGARM)
{
}

CSpringArm::~CSpringArm()
{
}

void CSpringArm::begin()
{
	auto cam = CRenderMgr::GetInst()->GetMainCam();
	SetTargetCamera(cam);
}

void CSpringArm::tick()
{
	if (!GetOwner()
		|| !Transform()
		|| !GetOwner()->GetParent()
		|| !GetOwner()->GetParent()->Transform()
		|| !m_pTargetCam) return;

	//Transform()->SetRelativePos(Vec3(0, 0, 0));

}

void CSpringArm::SetTargetCamera(CCamera* _cam)
{
	m_pTargetCam = _cam;
}
