#include "pch.h"
#include "CAfterImage.h"

CAfterImage::CAfterImage()
	: CScript((UINT)SCRIPT_TYPE::AFTERIMAGE)
{
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

	Ptr<CMaterial> pMtrl = GetOwner()->MeshRender()->GetMaterial(0);
	pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(UpdateDir, 1.f));
}
