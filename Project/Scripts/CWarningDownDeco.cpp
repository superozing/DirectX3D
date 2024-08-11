#include "pch.h"
#include "CWarningDownDeco.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include "CBossLV.h"

CWarningDownDeco::CWarningDownDeco()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::WARNINGDOWNDECO)
	, m_BossLV(nullptr)
	, m_LVState((int)BossLV_STATE::END)
{
}

CWarningDownDeco::~CWarningDownDeco()
{
}

void CWarningDownDeco::begin()
{
	CAtlasImageUIScript::begin();

	m_UIImg = CAssetMgr::GetInst()->Load<CTexture>(TEXIngame_Warning_Deco_1);
	SetAtlasSize(Vec2(0.f, 0.f), Vec2((UINT)2000, m_UIImg->GetHeight()));
	m_vBlendColor.w = 0.f;

	m_BossLV = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"GameMode");
}

void CWarningDownDeco::tick()
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
	else if ((int)BossLV_STATE::PlayingDelay == m_LVState)
	{
		float duration = GETBOSSLV->GetPlayingDelayTime();
		float accTime  = GETBOSSLV->GetAccTime();

		float fClamp	= RoRMath::ClampFloat(accTime / duration, 0.f, 1.f);
		float fLerp		= RoRMath::Lerp(0.f, 1.f, fClamp);
		m_vBlendColor.w = 1 - fLerp;
	}

	m_vLeftTop.x += 1.f;
}
