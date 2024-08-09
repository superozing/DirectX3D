#include "pch.h"
#include "CTutorialGameMode.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CEventListener.h"
#include "CArona.h"

static string state = "";

CTutorialGameMode::CTutorialGameMode()
	: CGameMode((UINT)SCRIPT_TYPE::TUTORIALGAMEMODE)
{
	m_FSM = new CRoRStateMachine<CTutorialGameMode>(this, (UINT)TutorialState::END);

	FSMInit(TutorialState, CTutorialGameMode, BasicMove);
	FSMInit(TutorialState, CTutorialGameMode, Dash);
	FSMInit(TutorialState, CTutorialGameMode, Shooting);
	FSMInit(TutorialState, CTutorialGameMode, CoverHigh);
	FSMInit(TutorialState, CTutorialGameMode, CoverJump);
	FSMInit(TutorialState, CTutorialGameMode, CoverLow);
	FSMInit(TutorialState, CTutorialGameMode, Ending);

	AppendScriptParam("State", SCRIPT_PARAM::STRING, &state, 0.f, 0.f, true);

	for (size_t i = 0; i < (size_t)TutorialState::END; i++)
	{
		AppendScriptParam(ToString(magic_enum::enum_name((TutorialState)i)), SCRIPT_PARAM::BOOL, &m_arrIsClear[i]);
	}
}

CTutorialGameMode::~CTutorialGameMode()
{
	if (m_FSM)
		delete m_FSM;
}

void CTutorialGameMode::begin()
{
	m_FSM->Begin();

	m_pArona = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(AronaName)->GetScript<CArona>();
	m_pWall	 = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"WALL_SHOOT");
	
	m_arrIsClear[0] = true;
	m_arrIsClear[1] = true;
	m_arrIsClear[2] = true;
	
	// TutorialGameModeSound init
	m_vecTutorialGameModeSound.resize((UINT)TutorialGameModeSoundType::END);

	m_vecTutorialGameModeSound[(UINT)TutorialGameModeSoundType::TutorialStart] =
		CAssetMgr::GetInst()->Load<CSound>(SNDUI_START_01);
	m_vecTutorialGameModeSound[(UINT)TutorialGameModeSoundType::BGM] = CAssetMgr::GetInst()->Load<CSound>(SNDKaiten_Screw_BGM);
	m_vecTutorialGameModeSound[(UINT)TutorialGameModeSoundType::DoorOpen] = CAssetMgr::GetInst()->Load<CSound>(SNDSFX_Door_Open);

	m_vecTutorialGameModeSound[(UINT)TutorialGameModeSoundType::TutorialStart]->Play(1);
	m_vecTutorialGameModeSound[(UINT)TutorialGameModeSoundType::BGM]->Play(0, .4f);

}

void CTutorialGameMode::tick()
{
	m_FSM->Update();
	state = magic_enum::enum_name((TutorialState)m_FSM->GetCurState());
}

void CTutorialGameMode::BasicMoveBegin()
{
}

int CTutorialGameMode::BasicMoveUpdate()
{
	// TODO : 충분한 기본적인 움직임(WASD), 마우스 움직임 검사하고 dasheventlistner 활성화 시켜줘야 함
	if (IsClear(TutorialState::BasicMove))
	{
		return m_FSM->GetCurState() + 1;
	}

	return m_FSM->GetCurState();
}

void CTutorialGameMode::BasicMoveEnd()
{
}

void CTutorialGameMode::DashBegin()
{
}

int CTutorialGameMode::DashUpdate()
{
	// TODO : 끝에 도달하면 수고했다는 메시지랑 다음 길 안내정도 스크립트 띄워주면 좋을듯
	if (IsClear(TutorialState::Dash))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CTutorialGameMode::DashEnd()
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

	if (IsClear(TutorialState::Shooting))
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

	// 문 열리는 효과음
	m_vecTutorialGameModeSound[(UINT)TutorialGameModeSoundType::DoorOpen]->Play(1, 1.f);

	m_pArona->Message("Congratulations!", 340, 3.f);
}

void CTutorialGameMode::CoverHighBegin()
{
}

int CTutorialGameMode::CoverHighUpdate()
{
	if (IsClear(TutorialState::CoverHigh))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CTutorialGameMode::CoverHighEnd()
{
}

void CTutorialGameMode::CoverJumpBegin()
{
}

int CTutorialGameMode::CoverJumpUpdate()
{
	if (IsClear(TutorialState::CoverJump))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CTutorialGameMode::CoverJumpEnd()
{
}

void CTutorialGameMode::CoverLowBegin()
{
}

int CTutorialGameMode::CoverLowUpdate()
{
	if (IsClear(TutorialState::CoverJump))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CTutorialGameMode::CoverLowEnd()
{
}

void CTutorialGameMode::EndingBegin()
{
}

int CTutorialGameMode::EndingUpdate()
{
	if (IsClear(TutorialState::Ending))
	{
		return m_FSM->GetCurState() + 1;
	}
	return m_FSM->GetCurState();
}

void CTutorialGameMode::EndingEnd()
{
	GamePlayStatic::ChangeLevel(CLevelMgr::GetInst()->LevelLoadFunc(LEVELTitle), LEVEL_STATE::PLAY);
}

void CTutorialGameMode::Clear(TutorialState _state)
{
	m_arrIsClear[(UINT)_state] = true;
}

bool CTutorialGameMode::IsClear(TutorialState _state)
{
	return m_arrIsClear[(UINT)_state];
}
