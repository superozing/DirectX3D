#include "pch.h"
#include "CTutStartTex.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>

#include "CTutorialGameMode.h"

CTutStartTex::CTutStartTex()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::TUTSTARTTEX)
	, m_pGM(nullptr)
	, m_LVState((int)TutorialState::END)
	, m_StartIn(false)
	, m_StartDelay(false)
	, m_StartOut(false)
{
}

CTutStartTex::~CTutStartTex()
{
}

void CTutStartTex::begin()
{
	CAtlasImageUIScript::begin();

	m_UIImg = CAssetMgr::GetInst()->Load<CTexture>(TEXIngame_Image_Start);
	SetAtlasSize(Vec2(0.f, 0.f), Vec2(m_UIImg->GetWidth(), m_UIImg->GetHeight()));
	m_vBlendColor.w = 10.f;

	Transform()->SetRelativePos(Vec3(-2000.f, 0.f, -10.f));

	if (GetOwner()->GetParent())
		m_pGM = GetOwner()->GetParent();
}

void CTutStartTex::tick()
{
	CAtlasImageUIScript::tick();

	m_LVState = m_pGM->GetScript<CTutorialGameMode>()->GetCurState(); //->GetCurLVState();

	if ((int)TutorialState::OpeningIn == m_LVState && !m_StartIn)
	{
		Transform()->Lerp(Vec3(-100.f, 0.f, -10.f), false, Vec3(0.f, 0.f, 0.f), false, Vec3(0.f, 0.f, 0.f), 0.4f);

		m_StartIn = true;
	}
	else if ((int)TutorialState::OpeningDelay == m_LVState && !m_StartDelay)
	{
		Transform()->Lerp(Vec3(100.f, 0.f, -10.f), false, Vec3(0.f, 0.f, 0.f), false, Vec3(0.f, 0.f, 0.f), 1.f);

		m_StartDelay = true;
	}
	else if ((int)TutorialState::OpeningOut == m_LVState && !m_StartOut)
	{
		Transform()->Lerp(Vec3(1910.f, 0.f, -10.f), false, Vec3(0.f, 0.f, 0.f), false, Vec3(0.f, 0.f, 0.f), 0.2f);

		m_StartOut = true;
	}
}
