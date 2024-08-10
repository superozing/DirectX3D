﻿#include "pch.h"
#include "CTutorialGameMode.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CPlayerScript.h"
#include "CPlayerController.h"
#include "CEventListener.h"
#include "CArona.h"

static string state = "";

CTutorialGameMode::CTutorialGameMode()
	: CGameMode((UINT)SCRIPT_TYPE::TUTORIALGAMEMODE)
	, m_fStopTimeLength(0.5f)
	, m_fTargetDistance(3000.f)
	, m_iDashCnt(1)
{
	m_FSM = new CRoRStateMachine<CTutorialGameMode>(this, (UINT)TutorialState::END);

	FSMInit(TutorialState, CTutorialGameMode, First);
	FSMInit(TutorialState, CTutorialGameMode, BasicMove);
	FSMInit(TutorialState, CTutorialGameMode, DashWait);
	FSMInit(TutorialState, CTutorialGameMode, Dash);
	FSMInit(TutorialState, CTutorialGameMode, ShootingWait);
	FSMInit(TutorialState, CTutorialGameMode, Shooting);
	FSMInit(TutorialState, CTutorialGameMode, CoverHighWait);
	FSMInit(TutorialState, CTutorialGameMode, CoverHigh);
	FSMInit(TutorialState, CTutorialGameMode, CoverJumpWait);
	FSMInit(TutorialState, CTutorialGameMode, CoverJump);
	FSMInit(TutorialState, CTutorialGameMode, CoverLowWait);
	FSMInit(TutorialState, CTutorialGameMode, CoverLow);
	FSMInit(TutorialState, CTutorialGameMode, EndingWait);
	FSMInit(TutorialState, CTutorialGameMode, Ending);

	AppendScriptParam("State", SCRIPT_PARAM::STRING, &state, 0.f, 0.f, true);

	for (size_t i = 0; i < (size_t)TutorialEvents::END; i++)
	{
		AppendScriptParam(ToString(magic_enum::enum_name(TutorialEvents(i))), SCRIPT_PARAM::BOOL, &m_arrIsClear[i]);
	}

	// 움직임 이벤트 체크
	AppendSeperateLine();
	AppendScriptParam("StopTimer", SCRIPT_PARAM::FLOAT, &m_fStopTimeLength);
	AppendScriptParam("Front", SCRIPT_PARAM::BOOL, &m_bMoveFront);
	AppendScriptParam("Back", SCRIPT_PARAM::BOOL, &m_bMoveBack);
	AppendScriptParam("Left", SCRIPT_PARAM::BOOL, &m_bMoveLeft);
	AppendScriptParam("Right", SCRIPT_PARAM::BOOL, &m_bMoveRight);
	AppendScriptParam("LeftDistance", SCRIPT_PARAM::FLOAT, &m_fTargetDistance);

	// 대쉬 이벤트 체크
	AppendSeperateLine();
	AppendScriptParam("DashCnt", SCRIPT_PARAM::INT, &m_iDashCnt);
}

CTutorialGameMode::~CTutorialGameMode()
{
	if (m_FSM)
		delete m_FSM;
}

void CTutorialGameMode::FirstBegin()
{
}

int CTutorialGameMode::FirstUpdate()
{
	return (int)TutorialState::BasicMove;
}

void CTutorialGameMode::FirstEnd()
{
}

void CTutorialGameMode::begin()
{
	CLevel* pLevel	= CLevelMgr::GetInst()->GetCurrentLevel();
	m_pArona		= pLevel->FindObjectByName(AronaName)->GetScript<CArona>();
	m_pWall			= pLevel->FindObjectByName(L"WALL_SHOOT");
	m_pPlayer		= pLevel->FindObjectByName(PlayerName);
	m_pPlayerScript = m_pPlayer->GetScript<CPlayerScript>();

	m_pBGM = CAssetMgr::GetInst()->Load<CSound>(SNDKaiten_Screw_BGM);
	m_pBGM->Play(0, 1.f);

	m_pEvents[(UINT)TutorialEvents::Dash]	  = pLevel->FindObjectByName(L"DASH EVENT")->GetScript<CEventListener>();
	m_pEvents[(UINT)TutorialEvents::Shooting] = pLevel->FindObjectByName(L"SHOOT EVENT")->GetScript<CEventListener>();
	m_pEvents[(UINT)TutorialEvents::CoverHigh] =
		pLevel->FindObjectByName(L"COVER HIGH EVENT")->GetScript<CEventListener>();
	m_pEvents[(UINT)TutorialEvents::CoverJump] =
		pLevel->FindObjectByName(L"COVER JUMP EVENT")->GetScript<CEventListener>();
	m_pEvents[(UINT)TutorialEvents::CoverLow] =
		pLevel->FindObjectByName(L"COVER LOW EVENT")->GetScript<CEventListener>();

	m_FSM->Begin();
}

void CTutorialGameMode::tick()
{
	m_FSM->Update();
	state = magic_enum::enum_name((TutorialState)m_FSM->GetCurState());
}

void CTutorialGameMode::BasicMoveBegin()
{
	m_fStopTimer = 0.f;
	m_bMoveFront = false;
	m_bMoveLeft	 = false;
	m_bMoveBack	 = false;
	m_bMoveRight = false;

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
	info.Flip		   = KEY::KEY_END;
	info.Sensitivity   = 0;

	PrevPos = m_pPlayer->Transform()->GetRelativePos();

	CPlayerController::SetInfo(info);

	m_pArona->Message("Press W, A, S, D to Move", 440, -1.f);
}

int CTutorialGameMode::BasicMoveUpdate()
{
	// TODO : 충분한 기본적인 움직임(WASD), 마우스 움직임 검사하고 dasheventlistner 활성화 시켜줘야 함

	// 일정 시간 후 움직임
	m_fStopTimer += DT;
	if (m_fStopTimer >= m_fStopTimeLength)
		CPlayerController::SetInfo(m_tPlayerKeyInfo);
	else
	{
		return m_FSM->GetCurState();
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
		return (int)TutorialState::DashWait;
	}

	PrevPos = CurPos;

	return m_FSM->GetCurState();
}

void CTutorialGameMode::BasicMoveEnd()
{
	m_pArona->Message("Congratulations!", 340, 3.f);
	Clear(TutorialEvents::BasicMove);
	m_fTargetDistance = 0.f;
}

void CTutorialGameMode::DashWaitBegin()
{
}

int CTutorialGameMode::DashWaitUpdate()
{
	if (m_pEvents[(UINT)TutorialEvents::Dash]->HasTargets())
	{
		return (int)TutorialState::Dash;
	}

	return m_FSM->GetCurState();
}

void CTutorialGameMode::DashWaitEnd()
{
}

void CTutorialGameMode::DashBegin()
{
	m_pArona->Message("Press Space to Dash And JumpUp The Rayzer", 480, -1.f);
}

int CTutorialGameMode::DashUpdate()
{
	// TODO : 끝에 도달하면 수고했다는 메시지랑 다음 길 안내정도 스크립트 띄워주면 좋을듯
	auto pPlayerFSM = m_pPlayerScript->GetStateMachine();
	if (pPlayerFSM->StateBegin((int)PLAYER_STATE::SkillDash))
	{
		m_iDashCnt--;
	}

	if (m_iDashCnt <= 0)
	{
		return (int)TutorialState::ShootingWait;
	}
	return m_FSM->GetCurState();
}

void CTutorialGameMode::DashEnd()
{
	m_pArona->Message("Congratulations!", 340, 3.f);
	Clear(TutorialEvents::Dash);
}

void CTutorialGameMode::ShootingWaitBegin()
{
}

int CTutorialGameMode::ShootingWaitUpdate()
{
	if (m_pEvents[(UINT)TutorialEvents::Shooting]->HasTargets())
	{
		return (int)TutorialState::Shooting;
	}

	return m_FSM->GetCurState();
}

void CTutorialGameMode::ShootingWaitEnd()
{
}

void CTutorialGameMode::ShootingBegin()
{
}

int CTutorialGameMode::ShootingUpdate()
{
	// TODO : 서서쏴 하는 곳
	// 1. 바닥 2개 생성
	// 2. 공중 2개 생성
	// 3. 바닥, 공중 2개 생성

	if (IsClear(TutorialEvents::Shooting))
	{
		return m_FSM->GetCurState() + 1;
	}

	return m_FSM->GetCurState();
}

void CTutorialGameMode::ShootingEnd()
{
	Vec3 vPos	= m_pWall->Transform()->GetRelativePos();
	Vec3 vScale = m_pWall->Transform()->GetRelativeScale();
	vPos.x += vScale.z;
	m_pWall->Transform()->Lerp(vPos, false, Vec3(), false, Vec3(), 2.f);

	m_pArona->Message("Congratulations!", 340, 3.f);
}

void CTutorialGameMode::CoverHighWaitBegin()
{
}

int CTutorialGameMode::CoverHighWaitUpdate()
{
	return 0;
}

void CTutorialGameMode::CoverHighWaitEnd()
{
}

void CTutorialGameMode::CoverHighBegin()
{
}

int CTutorialGameMode::CoverHighUpdate()
{
	if (IsClear(TutorialEvents::CoverHigh))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CTutorialGameMode::CoverHighEnd()
{
}

void CTutorialGameMode::CoverJumpWaitBegin()
{
}

int CTutorialGameMode::CoverJumpWaitUpdate()
{
	return 0;
}

void CTutorialGameMode::CoverJumpWaitEnd()
{
}

void CTutorialGameMode::CoverJumpBegin()
{
}

int CTutorialGameMode::CoverJumpUpdate()
{
	if (IsClear(TutorialEvents::CoverJump))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CTutorialGameMode::CoverJumpEnd()
{
}

void CTutorialGameMode::CoverLowWaitBegin()
{
}

int CTutorialGameMode::CoverLowWaitUpdate()
{
	return 0;
}

void CTutorialGameMode::CoverLowWaitEnd()
{
}

void CTutorialGameMode::CoverLowBegin()
{
}

int CTutorialGameMode::CoverLowUpdate()
{
	if (IsClear(TutorialEvents::CoverJump))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CTutorialGameMode::CoverLowEnd()
{
}

void CTutorialGameMode::EndingWaitBegin()
{
}

int CTutorialGameMode::EndingWaitUpdate()
{
	return 0;
}

void CTutorialGameMode::EndingWaitEnd()
{
}

void CTutorialGameMode::EndingBegin()
{
}

int CTutorialGameMode::EndingUpdate()
{
	if (IsClear(TutorialEvents::Ending))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CTutorialGameMode::EndingEnd()
{
	GamePlayStatic::ChangeLevel(CLevelMgr::GetInst()->LevelLoadFunc(LEVELTitle), LEVEL_STATE::PLAY);
}

void CTutorialGameMode::Clear(TutorialEvents _state)
{
	m_arrIsClear[(UINT)_state] = true;
}

bool CTutorialGameMode::IsClear(TutorialEvents _state)
{
	return m_arrIsClear[(UINT)_state];
}
