#include "pch.h"
#include "CReloadUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CAtlasImageUIScript.h"
#include "CPlayerController.h"
#include "CPlayerScript.h"

CReloadUI::CReloadUI()
	: CScript((UINT)SCRIPT_TYPE::RELOADUI)
{
	AppendScriptParam("ReloadTime", SCRIPT_PARAM::FLOAT, &m_fReloadTime);
}

CReloadUI::~CReloadUI()
{
}

void CReloadUI::begin()
{
	const auto& childs = GetOwner()->GetChild();
	for (const auto& child : childs)
	{
		auto pScript = child->GetScript<CAtlasImageUIScript>();
		if (pScript)
		{
			m_vecBullets.push_back(pScript);
			pScript->Draw(false);
		}
	}

	m_fReloadTime = CLevelMgr::GetInst()
						->GetCurrentLevel()
						->FindObjectByName(PlayerName)
						->Animator3D()
						->GetAnimLength((int)PLAYER_STATE::NormalReload);

	m_tBlankBullet.LeftTop = Vec2(740, 348);
	m_tBlankBullet.Size	   = Vec2(20, 54);

	m_tFillBullet.LeftTop = Vec2(592, 589);
	m_tFillBullet.Size	  = Vec2(20, 54);

	GetOwner()->GetScript<CAtlasImageUIScript>()->Draw(false);
}

void CReloadUI::tick()
{
	static float Timer = m_fReloadTime;

	Timer += DT;

	int state = CLevelMgr::GetInst()
					->GetCurrentLevel()
					->FindObjectByName(PlayerName)
					->GetScript<CPlayerScript>()
					->GetCurState();

	// 재장전 시작하면 초기화
	if (!GetOwner()->GetScript<CAtlasImageUIScript>()->IsDraw() &&
		(state == (int)PLAYER_STATE::NormalReload || state == (int)PLAYER_STATE::KneelReload ||
		 state == (int)PLAYER_STATE::StandReload))
	{
		Timer = 0.f;
		GetOwner()->GetScript<CAtlasImageUIScript>()->Draw(true);

		for (const auto& script : m_vecBullets)
		{
			script->Draw(true);
			script->SetAtlasSize(m_tBlankBullet.LeftTop, m_tBlankBullet.Size);
		}
	}

	// 재장전 종료될 때 삭제
	if (GetOwner()->GetScript<CAtlasImageUIScript>()->IsDraw() && Timer >= m_fReloadTime)
	{
		GetOwner()->GetScript<CAtlasImageUIScript>()->Draw(false);
		for (const auto& script : m_vecBullets)
		{
			script->Draw(false);
			script->SetAtlasSize(m_tBlankBullet.LeftTop, m_tBlankBullet.Size);
		}
	}

	// 하나씩 총알 이미지 변경
	if (Timer < m_fReloadTime)
	{
		int idx = Timer / m_fReloadTime * m_vecBullets.size();
		m_vecBullets[idx]->SetAtlasSize(m_tFillBullet.LeftTop, m_tFillBullet.Size);
	}
}
