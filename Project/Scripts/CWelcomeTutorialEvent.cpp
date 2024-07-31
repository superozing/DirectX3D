#include "pch.h"
#include "CWelcomeTutorialEvent.h"

#include "CAtlasImageUIScript.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CPlayerController.h"

CWelcomeTutorialEvent::CWelcomeTutorialEvent()
	: CEventListener((UINT)SCRIPT_TYPE::WELCOMETUTORIALEVENT)
	, m_fStopTimeLength(3.f)
{
	AppendScriptParam("StopTimer", SCRIPT_PARAM::FLOAT, &m_fStopTimeLength);
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
		m_pArona = arona->GetScript<CAtlasImageUIScript>();
		if (!m_pArona)
		{
			MessageBox(nullptr, L"아로나가 없습니다.", L"WelcomeTutorialEvent Error", 0);
		}
	}

	m_fStopTimer  = 0.f;
	m_bWalkAllow  = false;
	m_bTabW		  = false;
	m_bTabA		  = false;
	m_bTabS		  = false;
	m_bTabD		  = false;
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

	CPlayerController::SetInfo(info);
}

void CWelcomeTutorialEvent::tick()
{
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

	if (KEY_TAP(_1))
	{
	}
}

#define TagStopTimer "[StopTimer]"

void CWelcomeTutorialEvent::SaveToFile(ofstream& fout)
{
	fout << TagStopTimer << endl;
	fout << m_fStopTimeLength << endl;
}

void CWelcomeTutorialEvent::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagStopTimer);
	fin >> m_fStopTimeLength;
}
