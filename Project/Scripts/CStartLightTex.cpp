#include "pch.h"
#include "CStartLightTex.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include "CBossLV.h"

CStartLightTex::CStartLightTex()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::STARTLIGHTTEX)
	, m_BossLV(nullptr)
	, m_LVState((int)BossLV_STATE::END)
{
}

CStartLightTex::~CStartLightTex()
{
}

void CStartLightTex::begin()
{
	CAtlasImageUIScript::begin();

	m_UIImg = CAssetMgr::GetInst()->Load<CTexture>(TEXIngame_Start_Light);
	SetAtlasSize(Vec2(0.f, 0.f), Vec2(m_UIImg->GetWidth(), m_UIImg->GetHeight()));
	m_vBlendColor.w = 0.f;

	m_BossLV = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"GameMode");
}

void CStartLightTex::tick()
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
	else if ((int)BossLV_STATE::OpeningOut == m_LVState)
	{
		m_vBlendColor.w = 0.f;
	}
}
