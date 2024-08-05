#include "pch.h"
#include "CButtons.h"

#include "CBtnUIScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

CButtons::CButtons()
	: CScript((UINT)SCRIPT_TYPE::BUTTONS)
{
}

CButtons::~CButtons()
{
}

void CButtons::AddIdx()
{
	m_iIdx = RoRMath::ClampInt(m_iIdx + 1, 0, m_vecButtons.size());
	if (m_iIdx == m_vecButtons.size())
		m_iIdx = 0;
}

void CButtons::SubIdx()
{
	if (m_iIdx == 0)
		m_iIdx = m_vecButtons.size();

	m_iIdx = RoRMath::ClampInt(m_iIdx + -1, 0, m_vecButtons.size() - 1);
}

void CButtons::begin()
{
	const auto& childs = GetOwner()->GetChild();
	for (const auto& child : childs)
	{
		m_vecButtons.push_back(child->GetScript<CBtnUIScript>());
	}

	m_iIdx = 0;
}

void CButtons::tick()
{
	if (KEY_TAP(UP))
	{
		AddIdx();
	}
	if (KEY_TAP(DOWN))
	{
		SubIdx();
	}

	if (KEY_TAP(LBTN))
	{
		auto pLevel = CLevelMgr::GetInst()->LevelLoadFunc(LEVELTutPlace);
		GamePlayStatic::ChangeLevel(pLevel, LEVEL_STATE::PLAY);
	}
}
