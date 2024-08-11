#include "pch.h"
#include "CVictoryTex.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include "CBossLV.h"

CVictoryTex::CVictoryTex()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::VICTORYTEX)
	, m_BossLV(nullptr)
	, m_LVState((int)BossLV_STATE::END)
	, m_EndingIn(false)
	, m_EndingOut(false)
{
}

CVictoryTex::~CVictoryTex()
{
}

void CVictoryTex::begin()
{
	CAtlasImageUIScript::begin();

	m_UIImg = CAssetMgr::GetInst()->Load<CTexture>(TEXCampaign_Imgfont_Victory);
	SetAtlasSize(Vec2(0.f, 0.f), Vec2(m_UIImg->GetWidth(), m_UIImg->GetHeight()));

	m_BossLV = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"GameMode");

	Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
}

void CVictoryTex::tick()
{
	CAtlasImageUIScript::tick();

	m_LVState = GETBOSSLV->GetCurLVState();

	if ((int)BossLV_STATE::EndingDelay == m_LVState && !m_EndingIn)
	{
		Transform()->Lerp(Transform()->GetRelativePos(), false, Transform()->GetRelativeRotation(), true,
						  Vec3(1024.f, 256.f, 0.f), 0.1f);

		m_EndingIn = true;
	}
	else if ((int)BossLV_STATE::EndingOut == m_LVState && !m_EndingOut)
	{
		Transform()->Lerp(Transform()->GetRelativePos(), false, Transform()->GetRelativeRotation(), true,
						  Vec3(0.f, 0.f, 0.f), 0.1f);

		m_EndingOut = true;
	}
}
