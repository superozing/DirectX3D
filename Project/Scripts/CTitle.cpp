#include "pch.h"
#include "CTitle.h"

#include "CTitleTex.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CRoRStateMachine.h"

static string state = "";
CTitle::CTitle()
	: CGameMode((UINT)SCRIPT_TYPE::TITLE)
	, m_fAlphaTime(5.f)
{
	m_FSM = new CRoRStateMachine<CTitle>(this, (UINT)TitleState::END);

	FSMInit(TitleState, CTitle, RoROpen);
	FSMInit(TitleState, CTitle, RoRClose);
	FSMInit(TitleState, CTitle, BAOpen);
	FSMInit(TitleState, CTitle, BAClose);
	FSMInit(TitleState, CTitle, MenuSelect);

	AppendScriptParam("State", SCRIPT_PARAM::STRING, &state, 0.f, 0.f, true);
	AppendScriptParam("Alpha Time", SCRIPT_PARAM::FLOAT, &m_fAlphaTime);
}

CTitle::~CTitle()
{
	if (m_FSM)
		delete m_FSM;
}

void CTitle::RoROpenBegin()
{
	m_pRemnants->Draw(true);
	Vec4 vColor = m_pRemnants->GetColor();
	vColor.w	= 0.f;
	m_pRemnants->SetColor(vColor);
}

int CTitle::RoROpenUpdate()
{
	if (KEY_TAP(LBTN))
	{
		return (int)TitleState::RoRClose;
	}

	Vec4 vColor = m_pRemnants->GetColor();
	vColor.w += DT / m_fAlphaTime;
	m_pRemnants->SetColor(vColor);
	if (vColor.w >= 1.f)
	{
		return (int)TitleState::RoRClose;
	}

	return m_FSM->GetCurState();
}

void CTitle::RoROpenEnd()
{
}

void CTitle::RoRCloseBegin()
{
	Vec4 vColor = m_pRemnants->GetColor();
	vColor.w	= 1.f;
	m_pRemnants->SetColor(vColor);
}

int CTitle::RoRCloseUpdate()
{
	if (KEY_TAP(LBTN))
	{
		return (int)TitleState::BAOpen;
	}

	Vec4 vColor = m_pRemnants->GetColor();
	vColor.w -= DT / m_fAlphaTime;
	m_pRemnants->SetColor(vColor);
	if (vColor.w <= 0.f)
	{
		return (int)TitleState::BAOpen;
	}

	return m_FSM->GetCurState();
}

void CTitle::RoRCloseEnd()
{
	m_pRemnants->Draw(false);
}

void CTitle::BAOpenBegin()
{
	m_pBlueArchive->Draw(true);
	Vec4 vColor = m_pBlueArchive->GetColor();
	vColor.w	= 0.f;
	m_pBlueArchive->SetColor(vColor);
}

int CTitle::BAOpenUpdate()
{
	if (KEY_TAP(LBTN))
	{
		return (int)TitleState::BAClose;
	}

	Vec4 vColor = m_pBlueArchive->GetColor();
	vColor.w += DT / m_fAlphaTime;
	m_pBlueArchive->SetColor(vColor);
	if (vColor.w >= 1.f)
	{
		return (int)TitleState::BAClose;
	}

	return m_FSM->GetCurState();
}

void CTitle::BAOpenEnd()
{
}

#include "Engine\CSound.h"
void CTitle::BACloseBegin()
{
	Vec4 vColor = m_pRemnants->GetColor();
	vColor.w	= 0.f;
	m_pRemnants->SetColor(vColor);

	auto sound = CAssetMgr::GetInst()->Load<CSound>(SNDSerika_New_Year_Title);
	sound->Play(1, 1.f);
}

int CTitle::BACloseUpdate()
{
	if (KEY_TAP(LBTN))
	{
		return (int)TitleState::MenuSelect;
	}

	Vec4 vColor = m_pBlueArchive->GetColor();
	vColor.w -= DT / m_fAlphaTime;
	m_pBlueArchive->SetColor(vColor);
	if (vColor.w <= 0.f)
	{
		return (int)TitleState::MenuSelect;
	}

	return m_FSM->GetCurState();
}

void CTitle::BACloseEnd()
{
	m_pBlueArchive->Draw(false);
	GamePlayStatic::ChangeLevel(CLevelMgr::GetInst()->LevelLoadFunc(LEVELSelectMenu), LEVEL_STATE::PLAY);
}

void CTitle::MenuSelectBegin()
{
}

int CTitle::MenuSelectUpdate()
{
	return m_FSM->GetCurState();
}

void CTitle::MenuSelectEnd()
{
}

void CTitle::begin()
{
	const auto& childs = GetOwner()->GetChild();
	for (auto& child : childs)
	{
		auto script = child->GetScript<CTitleTex>();
		if (script)
		{
			if (child->GetName() == L"Remnants")
			{
				m_pRemnants = script;
			}
			if (child->GetName() == L"BlueArchive")
			{
				m_pBlueArchive = script;
			}
		}
	}
	m_pRemnants->Draw(false);
	m_pBlueArchive->Draw(false);

	m_FSM->Begin();

	// 사운드 로드
	CAssetMgr::GetInst()->Load<CSound>(SNDSerika_New_Year_Title);
}

void CTitle::tick()
{
	m_FSM->Update();
	state = magic_enum::enum_name((TitleState)m_FSM->GetCurState());
}