#include "pch.h"
#include "CVictoryBG.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include "CBossLV.h"

CVictoryBG::CVictoryBG()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::VICTORYBG)
	, m_BossLV(nullptr)
	, m_LVState((int)BossLV_STATE::END)
{
}

CVictoryBG::~CVictoryBG()
{
}

void CVictoryBG::begin()
{
	CAtlasImageUIScript::begin();

	m_UIImg = CAssetMgr::GetInst()->Load<CTexture>(TEXModal_Bg);
	SetAtlasSize(Vec2(0.f, 0.f), Vec2(m_UIImg->GetWidth(), m_UIImg->GetHeight()));
	m_vBlendColor.w = 0.f;

	m_BossLV = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"GameMode");
}

void CVictoryBG::tick()
{
	CAtlasImageUIScript::tick();

	m_LVState = GETBOSSLV->GetCurLVState();

	if ((int)BossLV_STATE::EndingIn == m_LVState)
	{
		float duration = GETBOSSLV->GetEndingInTime();
		float accTime  = GETBOSSLV->GetAccTime();

		float fLerp = RoRMath::Lerp(0.f, 1.f, accTime / duration);

		m_vBlendColor.w = fLerp;
	}
	else if ((int)BossLV_STATE::EndingOut == m_LVState)
	{
		float duration = GETBOSSLV->GetEndingOutTime();
		float accTime  = GETBOSSLV->GetAccTime();

		float fClamp = RoRMath::ClampFloat(accTime / duration, 0.f, 1.f);

		float fLerp = RoRMath::Lerp(0.f, 1.f, fClamp);

		m_vBlendColor.w = 1 - fLerp;
	}
}
