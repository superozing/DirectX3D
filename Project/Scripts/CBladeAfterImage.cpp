#include "pch.h"
#include "CBladeAfterImage.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include "CAfterImage.h"

CBladeAfterImage::CBladeAfterImage()
	: CScript((UINT)SCRIPT_TYPE::BLADEAFTERIMAGE)
	, m_Kaiten(nullptr)
{
	bSet = false;
}

CBladeAfterImage::~CBladeAfterImage()
{
}

void CBladeAfterImage::begin()
{
	m_Kaiten = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Kaiten");

	vector<CGameObject*> vecObj = GetOwner()->GetChild();

	for (int i = 0; i < vecObj.size(); ++i)
	{
		m_AfterImage = vecObj[i]->GetScript<CAfterImage>();

		if (m_AfterImage != nullptr)
			break;
	}
}

void CBladeAfterImage::tick()
{
	Matrix BoneFrameMat = m_Kaiten->Animator3D()->FindBoneMat(L"Bone_WP_Blade");
	Transform()->SetFrameMat(BoneFrameMat);

	if (bSet == false && m_AfterImage != nullptr && m_Kaiten->Animator3D()->GetCurFrameIdx() > 67)
	{
		bSet = true;
		m_AfterImage->SetAlpha(0.65f);
	}

	if (m_Kaiten->Animator3D()->GetCurFrameIdx() > 120)
		GamePlayStatic::DestroyGameObject(GetOwner());
}
