#include "pch.h"
#include "CBladeAfterImage.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>

CBladeAfterImage::CBladeAfterImage()
	: CScript((UINT)SCRIPT_TYPE::BLADEAFTERIMAGE)
	, m_Kaiten(nullptr)
{
}

CBladeAfterImage::~CBladeAfterImage()
{
}

void CBladeAfterImage::begin()
{
	m_Kaiten = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Kaiten");
}

void CBladeAfterImage::tick()
{
	Matrix BoneFrameMat = m_Kaiten->Animator3D()->FindBoneMat(L"Bone_WP_Blade");
	Transform()->SetFrameMat(BoneFrameMat);

	if (m_Kaiten->Animator3D()->GetCurFrameIdx() > 120)
		GamePlayStatic::DestroyGameObject(GetOwner());
}
