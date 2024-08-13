#include "pch.h"
#include "CTutorialGameMode.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CMemoryPoolMgr.h>

#include "CPlayerScript.h"
#include "CPlayerController.h"
#include "CEventListener.h"
#include "CSpawnSpotScript.h"
#include "CTutorialTarget.h"
#include "CArona.h"
#include "CFinishBalloon.h"
#include "CTurret.h"
#include "CDroidAR.h"
#include "CMemoryPoolMgrScript.h"

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
	AppendScriptParam("Basic Move", SCRIPT_PARAM::STRING, nullptr);
	AppendScriptParam("StopTimer", SCRIPT_PARAM::FLOAT, &m_fStopTimeLength);
	AppendScriptParam("Front", SCRIPT_PARAM::BOOL, &m_bMoveFront);
	AppendScriptParam("Back", SCRIPT_PARAM::BOOL, &m_bMoveBack);
	AppendScriptParam("Left", SCRIPT_PARAM::BOOL, &m_bMoveLeft);
	AppendScriptParam("Right", SCRIPT_PARAM::BOOL, &m_bMoveRight);
	AppendScriptParam("LeftDistance", SCRIPT_PARAM::FLOAT, &m_fTargetDistance);

	// 대쉬 이벤트 체크
	AppendSeperateLine();
	AppendScriptParam("Dash", SCRIPT_PARAM::STRING, nullptr);
	AppendScriptParam("DashCnt", SCRIPT_PARAM::INT, &m_iDashCnt);

	// 사격 이벤트 체크
	AppendSeperateLine();
	AppendScriptParam("Shooting", SCRIPT_PARAM::STRING, nullptr);
	for (int i = 0; i < SPAWNERCNT; i++)
	{
		string msg = "Target" + to_string(i + 1);
		AppendScriptParam(msg, SCRIPT_PARAM::BOOL, &m_arrIsMonsterDestroy[i]);
	}
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
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_pArona	   = pLevel->FindObjectByName(AronaName)->GetScript<CArona>();
	for (int i = 0; i < SPAWNERCNT; i++)
	{
		wstring name = L"TargetSpawner" + to_wstring(i + 1);
		m_vecTargetSpawners.push_back(pLevel->FindObjectByName(name)->GetScript<CSpawnSpotScript>());
		m_vecTargetSpawners[i]->RegisterObject();
		m_arrIsMonsterDestroy[i] = false;
	}

	m_pWall			 = pLevel->FindObjectByName(L"WALL_SHOOT");
	m_pPlayer		 = pLevel->FindObjectByName(PlayerName);
	m_pPlayerScript	 = m_pPlayer->GetScript<CPlayerScript>();
	m_pFinishBalloon = pLevel->FindObjectByName(L"Balloon")->GetScript<CFinishBalloon>();

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
	m_pEvents[(UINT)TutorialEvents::Ending] =
		pLevel->FindObjectByName(L"COVER CLEAR EVENT")->GetScript<CEventListener>();

	m_pArona = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(AronaName)->GetScript<CArona>();
	m_pWall	 = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"WALL_SHOOT");

	// for (int i = 0; i < 4; i++)
	//{
	//	wstring name   = L"Enemy" + to_wstring(i + 1);
	//	auto	pEnemy = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(name);
	//	m_vecCoverMonsters.push_back(pEnemy);
	// }

	// TutorialGameModeSound init
	m_vecTutorialGameModeSound.resize((UINT)TutorialGameModeSoundType::END);

	m_vecTutorialGameModeSound[(UINT)TutorialGameModeSoundType::TutorialStart] =
		CAssetMgr::GetInst()->Load<CSound>(SNDUI_START_01);
	m_vecTutorialGameModeSound[(UINT)TutorialGameModeSoundType::BGM] =
		CAssetMgr::GetInst()->Load<CSound>(SNDKaiten_Screw_BGM);
	m_vecTutorialGameModeSound[(UINT)TutorialGameModeSoundType::DoorOpen] =
		CAssetMgr::GetInst()->Load<CSound>(SNDSFX_Door_Open);

	m_vecTutorialGameModeSound[(UINT)TutorialGameModeSoundType::TutorialStart]->Play(1);
	m_vecTutorialGameModeSound[(UINT)TutorialGameModeSoundType::BGM]->Play(0, .4f);

	m_FSM->Begin();
	CKeyMgr::GetInst()->RoRShowCursor(false);
	m_FSM->SetCurState((int)TutorialState::CoverLowWait);
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
		GamePlayStatic::DestroyGameObject(m_pEvents[(UINT)TutorialEvents::Dash]->GetOwner());
		return (int)TutorialState::Dash;
	}

	return m_FSM->GetCurState();
}

void CTutorialGameMode::DashWaitEnd()
{
}

void CTutorialGameMode::DashBegin()
{
	m_pArona->Message("Press Space to Dash And JumpUp The Rayzer", 750, -1.f);
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
		GamePlayStatic::DestroyGameObject(m_pEvents[(UINT)TutorialEvents::Shooting]->GetOwner());
		return (int)TutorialState::Shooting;
	}

	return m_FSM->GetCurState();
}

void CTutorialGameMode::ShootingWaitEnd()
{
}

void CTutorialGameMode::ShootingBegin()
{
	m_pArona->Message("Remove all targets", 360, -1.f);
	for (size_t i = 0; i < SPAWNERCNT; i++)
	{
		m_vecTutorialTargets.push_back(m_vecTargetSpawners[i]->SpawnObject()->GetScript<CTutorialTarget>());
	}
}

int CTutorialGameMode::ShootingUpdate()
{
	// TODO : 서서쏴 하는 곳
	// 1. 바닥 2개 생성
	// 2. 공중 2개 생성
	// 3. 바닥, 공중 2개 생성

	bool bClear = true;
	for (size_t i = 0; i < SPAWNERCNT; i++)
	{
		bClear &= m_arrIsMonsterDestroy[i]; //= m_vecTutorialTargets[i]->IsHit();
	}

	if (bClear)
	{
		return (int)TutorialState::CoverHighWait;
	}

	return m_FSM->GetCurState();
}

void CTutorialGameMode::ShootingEnd()
{
	Vec3 vPos	= m_pWall->Transform()->GetRelativePos();
	Vec3 vScale = m_pWall->Transform()->GetRelativeScale();
	vPos.x += vScale.z;
	m_pWall->Transform()->Lerp(vPos, false, Vec3(), false, Vec3(), 2.f);

	// 문 열리는 효과음
	m_vecTutorialGameModeSound[(UINT)TutorialGameModeSoundType::DoorOpen]->Play(1, 1.f);

	m_pArona->Message("Congratulations!", 340, 3.f);
	Clear(TutorialEvents::Shooting);
}

void CTutorialGameMode::CoverHighWaitBegin()
{
}

int CTutorialGameMode::CoverHighWaitUpdate()
{
	if (m_pEvents[(UINT)TutorialEvents::CoverHigh]->HasTargets())
	{
		GamePlayStatic::DestroyGameObject(m_pEvents[(UINT)TutorialEvents::CoverHigh]->GetOwner());
		return (int)TutorialState::CoverHigh;
	}

	return m_FSM->GetCurState();
}

void CTutorialGameMode::CoverHighWaitEnd()
{
}

void CTutorialGameMode::CoverHighBegin()
{
	m_pArona->Message("Go to the Cover And Press LShift to Cover", 680.f);

	CGameObject* pObj	  = CAssetMgr::GetInst()->Load<CPrefab>(PREFTurret)->Instantiate();
	int			 LayerIdx = pObj->GetLayerIdx();

	pObj->Transform()->SetRelativePos(Vec3(-28.f, -109.f, 12930.f));
	pObj->SetName("Enemy1");
	GamePlayStatic::SpawnGameObject(pObj, LayerIdx, true);

	m_vecCoverMonsters.push_back(pObj);

	pObj = CAssetMgr::GetInst()->Load<CPrefab>(PREFTurret)->Instantiate();

	pObj->Transform()->SetRelativePos(Vec3(-28.f, -109.f, 14254.f));
	pObj->SetName("Enemy2");
	GamePlayStatic::SpawnGameObject(pObj, LayerIdx, true);

	m_vecCoverMonsters.push_back(pObj);
}

int CTutorialGameMode::CoverHighUpdate()
{
	if (m_vecCoverMonsters[0]->IsDead() && m_vecCoverMonsters[1]->IsDead())
	{
		return (int)TutorialState::CoverJumpWait;
	}

	return m_FSM->GetCurState();
}

void CTutorialGameMode::CoverHighEnd()
{
	Clear(TutorialEvents::CoverHigh);
}

void CTutorialGameMode::CoverJumpWaitBegin()
{
}

int CTutorialGameMode::CoverJumpWaitUpdate()
{
	if (m_pEvents[(UINT)TutorialEvents::CoverJump]->HasTargets())
	{
		GamePlayStatic::DestroyGameObject(m_pEvents[(UINT)TutorialEvents::CoverJump]->GetOwner());
		return (int)TutorialState::CoverJump;
	}
	return m_FSM->GetCurState();
}

void CTutorialGameMode::CoverJumpWaitEnd()
{
}

void CTutorialGameMode::CoverJumpBegin()
{
	m_pArona->Message("Go to Cover And Press Space to Jump", 650.f);
}

int CTutorialGameMode::CoverJumpUpdate()
{
	if (IsClear(TutorialEvents::CoverJump))
	{
		return (int)TutorialState::CoverJumpWait;
	}
	return m_FSM->GetCurState();
}

void CTutorialGameMode::CoverJumpEnd()
{
	Clear(TutorialEvents::CoverJump);
}

void CTutorialGameMode::CoverLowWaitBegin()
{
}

int CTutorialGameMode::CoverLowWaitUpdate()
{
	if (m_pEvents[(UINT)TutorialEvents::CoverLow]->HasTargets())
	{
		GamePlayStatic::DestroyGameObject(m_pEvents[(UINT)TutorialEvents::CoverLow]->GetOwner());
		return (int)TutorialState::CoverLow;
	}
	return m_FSM->GetCurState();
}

void CTutorialGameMode::CoverLowWaitEnd()
{
}

void CTutorialGameMode::CoverLowBegin()
{
	m_pArona->Message("Terminate Enemy", 350.f);

	CGameObject* pObj	  = CAssetMgr::GetInst()->Load<CPrefab>(PREFDroidAR)->Instantiate();
	int			 LayerIdx = pObj->GetLayerIdx();

	pObj->Transform()->SetRelativePos(Vec3(-28.f, -109.f, 16105.f));
	pObj->SetName("DroidAR1");
	GamePlayStatic::SpawnGameObject(pObj, LayerIdx, true);

	m_vecCoverLowMonsters.push_back(pObj);

	pObj = CAssetMgr::GetInst()->Load<CPrefab>(PREFDroidAR)->Instantiate();

	pObj->Transform()->SetRelativePos(Vec3(198.f, -109.f, 17443.f));
	pObj->SetName("DroidAR2");
	GamePlayStatic::SpawnGameObject(pObj, LayerIdx, true);

	m_vecCoverLowMonsters.push_back(pObj);
}

int CTutorialGameMode::CoverLowUpdate()
{
	if (m_vecCoverLowMonsters[0]->IsDead() && m_vecCoverLowMonsters[1]->IsDead())
		return (int)TutorialState::EndingWait;

	return m_FSM->GetCurState();
}

void CTutorialGameMode::CoverLowEnd()
{
	Clear(TutorialEvents::CoverLow);
}

void CTutorialGameMode::EndingWaitBegin()
{
	m_pFinishBalloon->Up();
}

int CTutorialGameMode::EndingWaitUpdate()
{
	if (m_pEvents[(UINT)TutorialEvents::Ending]->HasTargets())
	{
		GamePlayStatic::DestroyGameObject(m_pEvents[(UINT)TutorialEvents::Ending]->GetOwner());
		return (int)TutorialState::Ending;
	}
	return m_FSM->GetCurState();
}

void CTutorialGameMode::EndingWaitEnd()
{
}

void CTutorialGameMode::EndingBegin()
{
	m_pArona->Message("All Complete! You Finish!!!", 450.f);
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
