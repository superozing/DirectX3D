#include "pch.h"
#include "CWarningBGTex.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include "CBossLV.h"

CWarningBGTex::CWarningBGTex()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::WARNINGBGTEX)
	, m_BossLV(nullptr)
	, m_LVState((int)BossLV_STATE::END)
{
}

CWarningBGTex::~CWarningBGTex()
{
}

void CWarningBGTex::begin()
{
	CAtlasImageUIScript::begin();

	m_UIImg = CAssetMgr::GetInst()->Load<CTexture>(TEXModal_Bg);
	SetAtlasSize(Vec2(0.f, 0.f), Vec2(m_UIImg->GetWidth(), m_UIImg->GetHeight()));
	m_vBlendColor.w = 0.f;

	m_BossLV = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"GameMode");
}

void CWarningBGTex::tick()
{
	CAtlasImageUIScript::tick();

	m_LVState = GETBOSSLV->GetCurLVState();

	if ((int)BossLV_STATE::OpeningIn == m_LVState)
	{
		float duration = GETBOSSLV->GetOpeningInTime();
		float accTime  = GETBOSSLV->GetAccTime();

		float fLerp = RoRMath::Lerp(0.f, 1.f, accTime / duration);

		m_vBlendColor.w = fLerp;
	}
	else if ((int)BossLV_STATE::PlayingDelay == m_LVState)
	{
		float duration = GETBOSSLV->GetOpeningInTime();
		float accTime  = GETBOSSLV->GetAccTime();

		float fClamp = RoRMath::ClampFloat(accTime / duration, 0.f, 1.f);

		float fLerp = RoRMath::Lerp(0.f, 1.f, fClamp);

		m_vBlendColor.w = 1 - fLerp;
	}
}
