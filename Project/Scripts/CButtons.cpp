#include "pch.h"
#include "CButtons.h"

#include "CBtnUIScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CTextUI.h"
#include "Engine\CSound.h"

void (*CButtons::Setting)() = nullptr;

CButtons::CButtons()
	: CScript((UINT)SCRIPT_TYPE::BUTTONS)
{
}

CButtons::~CButtons()
{
}

#include "Engine\CRenderMgr.h"
void CButtons::Play()
{
	m_started  = true;
	auto sound = CAssetMgr::GetInst()->Load<CSound>(SNDMutsukiNew_Year_Battle_In_1);
	sound->Play(1, 1.f, true);
	CRenderMgr::GetInst()->PushFadeEvent(FADE_TYPE::FADE_OUT, 3.f);
}

void CButtons::Settings()
{
	if (CButtons::Setting)
		CButtons::Setting();
}

void CButtons::Exit()
{
	PostQuitMessage(0);
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
	m_vecButtons[(UINT)TitleButton::Play]->SetDeletage(this, (DelegateFunc)&CButtons::Play);
	m_vecButtons[(UINT)TitleButton::Settings]->SetDeletage(this, (DelegateFunc)&CButtons::Settings);
	m_vecButtons[(UINT)TitleButton::Exit]->SetDeletage(this, (DelegateFunc)&CButtons::Exit);

	m_iIdx		   = 0;
	m_started	   = false;
	m_LevelChanged = false;
	m_acctime	   = 0.f;
}

void CButtons::tick()
{
	if (true == m_started)
	{
		m_acctime += DT;
	}
	if (true == m_started && (m_acctime > m_ThresholdTime) && false == m_LevelChanged)
	{
		GamePlayStatic::ChangeLevel(CLevelMgr::GetInst()->LevelLoadFunc(LEVELTutPlace), LEVEL_STATE::PLAY);
		m_LevelChanged = true;
	}

	// if (KEY_TAP(UP))
	//{
	//	SubIdx();
	// }
	// if (KEY_TAP(DOWN))
	//{
	//	AddIdx();
	// }

	// if (KEY_TAP(ENTER))
	//{
	//	m_vecButtons[m_iIdx]->LBtnClicked();
	// }

	for (size_t i = 0; i < (UINT)TitleButton::END; i++)
	{
		if (m_vecButtons[i]->GetState() == ButtonState::Normal)
		{
			m_vecButtons[i]->GetOwner()->GetChild()[0]->GetScript<CTextUI>()->SetColor(Vec4(255, 255, 255, 255) /
																					   255.f);
		}
		else
		{
			m_vecButtons[i]->GetOwner()->GetChild()[0]->GetScript<CTextUI>()->SetColor(Vec4(122, 222, 252, 255) /
																					   255.f);
		}
	}
}
