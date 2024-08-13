#include "pch.h"
#include "CBossLV.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CRoRStateMachine.h"
#include "CPlayerScript.h"
#include "CFadeUIScript.h"

static string state = "";

CBossLV::CBossLV()
	: CGameMode((UINT)SCRIPT_TYPE::BOSSLV)
	, m_Player(nullptr)
	, m_HUD(nullptr)
	, m_OpeningInTime(0.5f)
	, m_OpeningDelayTime(1.f)
	, m_OpeningOutTime(0.5f)
	, m_PlayingInTime(3.f)
	, m_PlayingDelayTime(0.5f)
	, m_EndingInTime(0.5f)
	, m_EndingDelayTime(1.f)
	, m_EndingOutTime(0.5f)
	, m_EndingCutInTime(10.f)
	, m_Acctime(0.f)
	, m_BGM{}
	, m_SFX{}
{
	m_FSM = new CRoRStateMachine<CBossLV>(this, (UINT)BossLV_STATE::END);

	FSMInit(BossLV_STATE, CBossLV, FadeIn);
	FSMInit(BossLV_STATE, CBossLV, OpeningIn);
	FSMInit(BossLV_STATE, CBossLV, OpeningDelay);
	FSMInit(BossLV_STATE, CBossLV, OpeningOut);
	FSMInit(BossLV_STATE, CBossLV, PlayingIn);
	FSMInit(BossLV_STATE, CBossLV, PlayingDelay);
	FSMInit(BossLV_STATE, CBossLV, Playing);
	FSMInit(BossLV_STATE, CBossLV, PlayingOut);
	FSMInit(BossLV_STATE, CBossLV, EndingIn);
	FSMInit(BossLV_STATE, CBossLV, EndingDelay);
	FSMInit(BossLV_STATE, CBossLV, EndingOut);
	FSMInit(BossLV_STATE, CBossLV, EndingCutIn);
	FSMInit(BossLV_STATE, CBossLV, FadeOut);

	AppendScriptParam("State", SCRIPT_PARAM::STRING, &state, 0.f, 0.f, true);
}

CBossLV::CBossLV(const CBossLV& _Origin)
	: CGameMode(_Origin.GetScriptType())
	, m_Player(nullptr)
	, m_HUD(nullptr)
	, m_OpeningInTime(_Origin.m_OpeningInTime)
	, m_OpeningDelayTime(_Origin.m_OpeningDelayTime)
	, m_OpeningOutTime(_Origin.m_OpeningOutTime)
	, m_PlayingInTime(_Origin.m_PlayingInTime)
	, m_PlayingDelayTime(_Origin.m_PlayingDelayTime)
	, m_EndingInTime(_Origin.m_EndingInTime)
	, m_EndingDelayTime(_Origin.m_EndingDelayTime)
	, m_EndingOutTime(_Origin.m_EndingOutTime)
	, m_EndingCutInTime(_Origin.m_EndingCutInTime)
	, m_Acctime(0.f)
	, m_BGM{}
	, m_SFX{}
{
	m_FSM = new CRoRStateMachine<CBossLV>(this, (UINT)BossLV_STATE::END);

	FSMInit(BossLV_STATE, CBossLV, OpeningIn);
	FSMInit(BossLV_STATE, CBossLV, OpeningDelay);
	FSMInit(BossLV_STATE, CBossLV, OpeningOut);
	FSMInit(BossLV_STATE, CBossLV, PlayingIn);
	FSMInit(BossLV_STATE, CBossLV, PlayingDelay);
	FSMInit(BossLV_STATE, CBossLV, Playing);
	FSMInit(BossLV_STATE, CBossLV, PlayingOut);
	FSMInit(BossLV_STATE, CBossLV, EndingIn);
	FSMInit(BossLV_STATE, CBossLV, EndingDelay);
	FSMInit(BossLV_STATE, CBossLV, EndingOut);
	FSMInit(BossLV_STATE, CBossLV, EndingCutIn);

	AppendScriptParam("State", SCRIPT_PARAM::STRING, &state, 0.f, 0.f, true);
}

CBossLV::~CBossLV()
{
	if (m_FSM)
		delete m_FSM;
}

void CBossLV::FadeInBegin()
{
	m_Acctime = 0.f;
	m_FadeScript->Push_FadeEvent(FADE_TYPE::FADE_IN, m_FadeInTime);
}

int CBossLV::FadeInUpdate()
{
	m_Acctime += DT;

	if (m_Acctime >= m_FadeInTime)
	{
		return (UINT)BossLV_STATE::OpeningIn;
	}
	return (UINT)BossLV_STATE::FadeIn;
}

void CBossLV::FadeInEnd()
{
}

int CBossLV::GetCurLVState()
{
	return m_FSM->GetCurState();
}

void CBossLV::LoadSoundAsset()
{
	// BGM 로드
	m_BGM.resize((UINT)BossLV_BGM::END);

	Ptr<CSound> pSound					  = CAssetMgr::GetInst()->Load<CSound>(SNDKaiten_Screw_BGM);
	m_BGM[(UINT)BossLV_BGM::KAITEN_SCREW] = pSound;

	pSound								= CAssetMgr::GetInst()->Load<CSound>(SNDBGM_PartyTime);
	m_BGM[(UINT)BossLV_BGM::PARTY_TIME] = pSound;

	//=========================
	// SFX 로드
	m_SFX.resize((UINT)BossLV_SFX::END);

	pSound						   = CAssetMgr::GetInst()->Load<CSound>(SNDUI_START_01);
	m_SFX[(UINT)BossLV_SFX::START] = pSound;

	pSound							 = CAssetMgr::GetInst()->Load<CSound>(SNDUI_Alarm);
	m_SFX[(UINT)BossLV_SFX::WARNING] = pSound;

	pSound								 = CAssetMgr::GetInst()->Load<CSound>(SNDUI_Victory_ST_01);
	m_SFX[(UINT)BossLV_SFX::VICTORY_BGM] = pSound;

	pSound							 = CAssetMgr::GetInst()->Load<CSound>(SNDAzusa_Tactic_Victory_1);
	m_SFX[(UINT)BossLV_SFX::VICTORY] = pSound;
}

void CBossLV::begin()
{
	LoadSoundAsset();
	m_FadeScript = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"FadeObject")->GetScript<CFadeUIScript>();
	m_FSM->Begin();

	m_Player = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Azusa");
	m_Player->GetScript<CPlayerScript>()->SetPlayable(false);

	m_HUD = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"HUD");
	m_HUD->Transform()->SetRelativePos(Vec3(0.f, 20000.f, 100.f));

	m_Kaiten = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Kaiten");
}

void CBossLV::tick()
{
	m_FSM->Update();
	state = magic_enum::enum_name((BossLV_STATE)m_FSM->GetCurState());
}

void CBossLV::OpeningInBegin()
{
	m_SFX[(UINT)BossLV_SFX::START]->Play(1.f, 1.f);
}

int CBossLV::OpeningInUpdate()
{
	m_Acctime += DT;

	if (m_Acctime >= m_OpeningInTime)
	{
		return (int)BossLV_STATE::OpeningDelay;
	}

	return m_FSM->GetCurState();
}

void CBossLV::OpeningInEnd()
{
	m_Acctime = 0.f;
}

void CBossLV::OpeningDelayBegin()
{
	m_SFX[(UINT)BossLV_SFX::WARNING]->Play(1.f, 1.f);
}

int CBossLV::OpeningDelayUpdate()
{
	m_Acctime += DT;

	if (m_Acctime >= m_OpeningDelayTime)
	{
		return (int)BossLV_STATE::OpeningOut;
	}

	return m_FSM->GetCurState();
}

void CBossLV::OpeningDelayEnd()
{
	m_Acctime = 0.f;
}

void CBossLV::OpeningOutBegin()
{
}

int CBossLV::OpeningOutUpdate()
{
	m_Acctime += DT;

	if (m_Acctime >= m_OpeningOutTime)
	{
		return (int)BossLV_STATE::PlayingIn;
	}

	return m_FSM->GetCurState();
}

void CBossLV::OpeningOutEnd()
{
	m_Acctime = 0.f;
}

void CBossLV::PlayingInBegin()
{
}

int CBossLV::PlayingInUpdate()
{
	m_Acctime += DT;

	if (m_Acctime >= m_PlayingInTime)
	{
		return (int)BossLV_STATE::PlayingDelay;
	}

	return m_FSM->GetCurState();
}

void CBossLV::PlayingInEnd()
{
	m_Acctime = 0.f;
}

void CBossLV::PlayingDelayBegin()
{
}

int CBossLV::PlayingDelayUpdate()
{
	m_Acctime += DT;

	if (m_Acctime >= m_PlayingDelayTime)
	{
		return (int)BossLV_STATE::Playing;
	}

	return m_FSM->GetCurState();
}

void CBossLV::PlayingDelayEnd()
{
	m_Acctime = 0.f;
}

void CBossLV::PlayingBegin()
{
	m_Player->GetScript<CPlayerScript>()->SetPlayable(true);
	m_BGM[(UINT)BossLV_BGM::KAITEN_SCREW]->Play(0.f, 1.f);
	m_HUD->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
}

int CBossLV::PlayingUpdate()
{
	return m_FSM->GetCurState();
}

void CBossLV::PlayingEnd()
{
	m_BGM[(UINT)BossLV_BGM::KAITEN_SCREW]->Stop();
	m_HUD->Transform()->SetRelativeScale(Vec3(1.f, 0.f, 1.f));
}

void CBossLV::PlayingOutBegin()
{
}

int CBossLV::PlayingOutUpdate()
{
	return m_FSM->GetCurState();
}

void CBossLV::PlayingOutEnd()
{
}

void CBossLV::EndingInBegin()
{
}

int CBossLV::EndingInUpdate()
{
	m_Acctime += DT;

	if (m_Acctime >= m_EndingInTime)
	{
		return (int)BossLV_STATE::EndingDelay;
	}

	return m_FSM->GetCurState();
}

void CBossLV::EndingInEnd()
{
	m_SFX[(UINT)BossLV_SFX::VICTORY_BGM]->Play(1.f, 1.f);
	m_Acctime = 0.f;
}

void CBossLV::EndingDelayBegin()
{
}

int CBossLV::EndingDelayUpdate()
{
	m_Acctime += DT;

	if (m_Acctime >= m_EndingDelayTime)
	{
		return (int)BossLV_STATE::EndingOut;
	}

	return m_FSM->GetCurState();
}

void CBossLV::EndingDelayEnd()
{
	m_Acctime = 0.f;
}

void CBossLV::EndingOutBegin()
{
	m_SFX[(UINT)BossLV_SFX::VICTORY]->Play(1.f, 1.f);
}

int CBossLV::EndingOutUpdate()
{
	m_Acctime += DT;

	if (m_Acctime >= m_EndingOutTime)
	{
		return (int)BossLV_STATE::EndingCutIn;
	}

	return m_FSM->GetCurState();
}

void CBossLV::EndingOutEnd()
{
	m_Acctime = 0.f;
}

void CBossLV::EndingCutInBegin()
{
	m_BGM[(UINT)BossLV_BGM::PARTY_TIME]->Play(1.f, 1.f);

	Vec3 vPos = m_Kaiten->Transform()->GetWorldPos();
	Vec3 vDir = m_Kaiten->Transform()->GetWorldDir(DIR_TYPE::FRONT);

	Vec3 vNewPos = vPos + (vDir * 500.f);

	m_Player->Transform()->SetRelativePos(vNewPos);
	m_Player->Transform()->SetDir(vDir);
	m_Player->GetScript<CPlayerScript>()->GetStateMachine()->SetCurState((int)PLAYER_STATE::VictoryStart);
}

int CBossLV::EndingCutInUpdate()
{
	m_Acctime += DT;

	if (m_Acctime >= m_EndingCutInTime)
	{
		return (UINT)BossLV_STATE::FadeOut;
	}

	return m_FSM->GetCurState();
}

void CBossLV::EndingCutInEnd()
{
}

void CBossLV::FadeOutBegin()
{
	m_Acctime = 0.f;
	m_FadeScript->Push_FadeEvent(FADE_TYPE::FADE_OUT, m_FadeOutTime);
}

int CBossLV::FadeOutUpdate()
{
	m_Acctime += DT;
	if (m_Acctime >= m_FadeOutTime)
	{
		GamePlayStatic::ChangeLevel(CLevelMgr::GetInst()->LevelLoadFunc(LEVELSelectMenu), LEVEL_STATE::PLAY);
	}
	return (UINT)BossLV_STATE::FadeOut;
}

void CBossLV::FadeOutEnd()
{
}
