#include "pch.h"
#include "CWarningIcon.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include "CBossLV.h"

CWarningIcon::CWarningIcon()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::WARNINGICON)
	, m_BossLV(nullptr)
	, m_LVState((int)BossLV_STATE::END)
{
}

CWarningIcon::~CWarningIcon()
{
}

void CWarningIcon::begin()
{
	CAtlasImageUIScript::begin();

	m_UIImg = CAssetMgr::GetInst()->Load<CTexture>(TEXIngame_Warning_Deco_2);
	SetAtlasSize(Vec2(0.f, 0.f), Vec2(m_UIImg->GetWidth(), m_UIImg->GetHeight()));
	m_vBlendColor.w = 0.f;

	m_BossLV = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"GameMode");
}

void CWarningIcon::tick()
{
	CAtlasImageUIScript::tick();

	m_LVState = GETBOSSLV->GetCurLVState();

	if ((int)BossLV_STATE::OpeningDelay == m_LVState)
	{
		float duration = GETBOSSLV->GetOpeningInTime();
		float accTime  = GETBOSSLV->GetAccTime();

		float fClamp	= RoRMath::ClampFloat(accTime / duration, 0.f, 1.f);
		float fLerp		= RoRMath::Lerp(0.f, 1.f, fClamp);
		m_vBlendColor.w = fLerp;
	}
	else if ((int)BossLV_STATE::PlayingIn == m_LVState)
	{
		float accTime = GETBOSSLV->GetAccTime();

		float fLerp		= RoRMath::ClampFloat(cos(accTime * 3.f) * 0.5f + 0.5f, 0.f, 1.f);
		float LerpAlpha = RoRMath::Lerp(0.f, 1.f, fLerp);
		m_vBlendColor.w = LerpAlpha;
	}
	else if ((int)BossLV_STATE::PlayingDelay == m_LVState)
	{
		m_vBlendColor.w = RoRMath::ClampFloat(m_vBlendColor.w - DT * 2.f, 0.f, 1.f);
	}
}
