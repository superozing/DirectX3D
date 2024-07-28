#include "pch.h"
#include "CReloadUI.h"

#include <Engine\CGameObject.h>
#include "CAtlasImageUIScript.h"
#include "CPlayerController.h"

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

	// TODO : 사격시스템 들어오면 리로드 시간 받아야 함
	m_fReloadTime = 2.f;

	m_tBlankBullet.LeftTop = Vec2(740, 348);
	m_tBlankBullet.Size	   = Vec2(20, 55);

	m_tFillBullet.LeftTop = Vec2(592, 589);
	m_tFillBullet.Size	  = Vec2(20, 55);

	GetOwner()->GetScript<CAtlasImageUIScript>()->Draw(false);
}

void CReloadUI::tick()
{
	static float Timer = m_fReloadTime;

	Timer += DT;

	// TODO : 사격시스템 들어오면 변경해야 함
	if (KEY_TAP(CPlayerController::Reload))
	{
		Timer = 0.f;
		GetOwner()->GetScript<CAtlasImageUIScript>()->Draw(true);

		for (const auto& script : m_vecBullets)
		{
			script->Draw(true);
			script->SetAtlasSize(m_tBlankBullet.LeftTop, m_tBlankBullet.Size);
		}
	}

	if (GetOwner()->GetScript<CAtlasImageUIScript>()->IsDraw() && Timer >= m_fReloadTime)
	{
		GetOwner()->GetScript<CAtlasImageUIScript>()->Draw(false);
		for (const auto& script : m_vecBullets)
		{
			script->Draw(false);
			script->SetAtlasSize(m_tBlankBullet.LeftTop, m_tBlankBullet.Size);
		}
	}

	if (Timer < m_fReloadTime)
	{
		int idx = Timer / m_fReloadTime * m_vecBullets.size();
		m_vecBullets[idx]->SetAtlasSize(m_tFillBullet.LeftTop, m_tFillBullet.Size);
	}
}
