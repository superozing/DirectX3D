#include "pch.h"
#include "CWarningTex.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include "CBossLV.h"

CWarningTex::CWarningTex()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::WARNINGTEX)
	, m_BossLV(nullptr)
	, m_LVState((int)BossLV_STATE::END)
	, m_WarningIn(false)
	, m_WarningOut(false)
{
}

CWarningTex::~CWarningTex()
{
}
void CWarningTex::begin()
{
	CAtlasImageUIScript::begin();

	m_UIImg = CAssetMgr::GetInst()->Load<CTexture>(TEXIngame_Warning_Text_2);
	SetAtlasSize(Vec2(0.f, 0.f), Vec2(m_UIImg->GetWidth(), m_UIImg->GetHeight()));
	m_vBlendColor = Vec4(1.f, 0.5f, 0.f, 0.9f);

	m_BossLV = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"GameMode");

	Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
}

void CWarningTex::tick()
{
	CAtlasImageUIScript::tick();

	m_LVState = GETBOSSLV->GetCurLVState();

	// 크기 Lerp
	if ((int)BossLV_STATE::OpeningDelay == m_LVState && !m_WarningIn)
	{
		Transform()->Lerp(Transform()->GetRelativePos(), false, Transform()->GetRelativeRotation(), true,
						  Vec3(700.f, 130.f, 10.f), 0.1f);

		m_WarningIn = true;
	}
	else if ((int)BossLV_STATE::PlayingDelay == m_LVState && !m_WarningOut)
	{
		Transform()->Lerp(Transform()->GetRelativePos(), false, Transform()->GetRelativeRotation(), true,
						  Vec3(0.f, 0.f, 10.f), 0.1f);

		m_WarningOut = true;
	}

	// 색 Lerp
	if (m_WarningIn)
	{
		float accTime = GETBOSSLV->GetAccTime();

		float fLerp = RoRMath::ClampFloat(cos(accTime * 5.f) * 0.5f + 0.5f, 0.f, 1.f);

		float LerpColor = RoRMath::Lerp(0.5f, 0.8f, fLerp);

		m_vBlendColor.y = LerpColor;
	}
}
