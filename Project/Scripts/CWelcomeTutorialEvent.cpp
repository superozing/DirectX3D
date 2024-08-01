#include "pch.h"
#include "CWelcomeTutorialEvent.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CPlayerController.h"
#include "CTutorialGameMode.h"
#include "CArona.h"

static Vec3 PrevPos;
CWelcomeTutorialEvent::CWelcomeTutorialEvent()
	: CEventListener((UINT)SCRIPT_TYPE::WELCOMETUTORIALEVENT)
	, m_fStopTimeLength(3.f)
	, m_fTargetDistance(3000.f)
{
	AppendScriptParam("StopTimer", SCRIPT_PARAM::FLOAT, &m_fStopTimeLength);

	AppendScriptParam("Front", SCRIPT_PARAM::BOOL, &m_bMoveFront);
	AppendScriptParam("Back", SCRIPT_PARAM::BOOL, &m_bMoveBack);
	AppendScriptParam("Left", SCRIPT_PARAM::BOOL, &m_bMoveLeft);
	AppendScriptParam("Right", SCRIPT_PARAM::BOOL, &m_bMoveRight);
	AppendScriptParam("LeftDistance", SCRIPT_PARAM::FLOAT, &m_fTargetDistance);
}

CWelcomeTutorialEvent::~CWelcomeTutorialEvent()
{
}

void CWelcomeTutorialEvent::begin()
{

	auto arona = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(AronaName);
	if (!arona)
	{
		MessageBox(nullptr, L"아로나가 없습니다.", L"WelcomeTutorialEvent Error", 0);
	}
	else
	{
		m_pArona = arona->GetScript<CArona>();
		if (!m_pArona)
		{
			MessageBox(nullptr, L"아로나가 없습니다.", L"WelcomeTutorialEvent Error", 0);
		}
	}

	m_pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(PlayerName);

	m_fStopTimer  = 0.f;
	m_bWalkAllow  = false;
	m_bMoveFront  = false;
	m_bMoveLeft	  = false;
	m_bMoveBack	  = false;
	m_bMoveRight  = false;
	m_bWalkEnough = false;

	m_tPlayerKeyInfo   = CPlayerController::GetInfo();
	PlayerKeyInfo info = {};
	info.Front		   = KEY::KEY_END;
	info.Right		   = KEY::KEY_END;
	info.Back		   = KEY::KEY_END;
	info.Left		   = KEY::KEY_END;
	info.Dash		   = KEY::KEY_END;
	info.Attack		   = KEY::KEY_END;
	info.Zoom		   = KEY::KEY_END;
	info.Reload		   = KEY::KEY_END;
	info.Cover		   = KEY::KEY_END;
	info.Skill		   = KEY::KEY_END;
	info.Jump		   = KEY::KEY_END;
	info.Sensitivity   = 0;

	PrevPos = m_pPlayer->Transform()->GetRelativePos();

	CPlayerController::SetInfo(info);
}

void CWelcomeTutorialEvent::tick()
{
	// 첫 프레임에 바로
	if (!m_bStart)
	{
		m_bStart = true;
		m_pArona->Message("Press W, A, S, D to Move", 440, -1.f);
	}

	// 일정 시간 후 움직이기 시작
	if (!m_bWalkAllow)
	{
		m_fStopTimer += DT;

		if (m_fStopTimer >= m_fStopTimeLength)
		{
			m_bWalkAllow = true;
			CPlayerController::SetInfo(m_tPlayerKeyInfo);
		}
		return;
	}

	if (KEY_TAP(CPlayerController::Front))
		m_bMoveFront = true;
	if (KEY_TAP(CPlayerController::Left))
		m_bMoveLeft = true;
	if (KEY_TAP(CPlayerController::Back))
		m_bMoveBack = true;
	if (KEY_TAP(CPlayerController::Right))
		m_bMoveRight = true;

	bool AllMove = m_bMoveFront && m_bMoveLeft && m_bMoveBack && m_bMoveRight;

	Vec3 CurPos = m_pPlayer->Transform()->GetRelativePos();
	m_fTargetDistance -= Vec3::Distance(CurPos, PrevPos);
	if (AllMove && m_fTargetDistance <= 0.f)
	{
		m_pArona->Message("Congratulations!", 340, 3.f);
		m_fTargetDistance = 0.f;

		auto gm = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(TutGMName)->GetScript<CTutorialGameMode>();
		gm->Clear(TutorialState::BasicMove);

		GamePlayStatic::DestroyGameObject(GetOwner());
	}

	PrevPos = CurPos;
}

#define TagStopTimer "[StopTimer]"
#define TagTargetDistance "[TargetDistance]"

void CWelcomeTutorialEvent::SaveToFile(ofstream& fout)
{
	fout << TagStopTimer << endl;
	fout << m_fStopTimeLength << endl;

	fout << TagTargetDistance << endl;
	fout << m_fTargetDistance << endl;
}

void CWelcomeTutorialEvent::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagStopTimer);
	fin >> m_fStopTimeLength;

	Utils::GetLineUntilString(fin, TagTargetDistance);
	fin >> m_fTargetDistance;
}
