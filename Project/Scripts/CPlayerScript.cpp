﻿#include "pch.h"
#include "CPlayerScript.h"
#include <Engine/CAssetMgr.h>

#include <Engine/CGameObject.h>
#include "CMissileScript.h"

#include <Engine/CMaterial.h>
#include <Engine/CRenderComponent.h>

#include <Engine/CKeyMgr.h>
#include <Engine/CLogMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CRandomMgr.h>

#include "CRoRStateMachine.h"
#include "CPlayerController.h"
#include "CSpringArm.h"

#include "CShootingSystemScript.h"
#include "CPlayerDamagedScript.h"

#include "CCrosshair.h"
#include "CHUD.h"

static string state = "";
static string cover = "";

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_tStatus{}
	, m_pSpringArm(nullptr)
	, m_pMuzzleFlash(nullptr)
	, m_bPlaying(true)
{
	// 디버깅용
	AppendScriptParam("CurState", SCRIPT_PARAM::STRING, (void*)&state);
	AppendScriptParam("CoverType", SCRIPT_PARAM::STRING, (void*)&cover);

	// TODO : 완성하고 복사 생성자에 추가해야 프리팹에서 동작함
	// 스크립트 파람 초기화
	InitScriptParamUI();

	// 스테이트 초기화
	InitStateMachine();

	// 스프링 암 세팅 초기화
	InitSpringArmSetting();
}

CPlayerScript::CPlayerScript(const CPlayerScript& _origin)
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_tStatus(_origin.m_tStatus)
	, m_mSpringInfos(_origin.m_mSpringInfos)
	, m_bPlaying(_origin.m_bPlaying)
{
	// m_FSM = _origin.m_FSM->Clone(this);

	// 스크립트 파람 초기화
	InitScriptParamUI();

	// 스테이트 초기화
	InitStateMachine();

	// 스프링 암 세팅 초기화
	InitSpringArmSetting();
}

CPlayerScript::~CPlayerScript()
{
	if (nullptr != m_FSM)
	{
		delete m_FSM;
		m_FSM = nullptr;
	}
}

void CPlayerScript::InitScriptParamUI()
{
	AppendScriptParam("IsDead", SCRIPT_PARAM::BOOL, &m_tStatus.IsDead, 0, 0, true);
	AppendScriptParam("IsInvincibility", SCRIPT_PARAM::BOOL, &m_tStatus.Invincibility, 0, 0, true);

	AppendScriptParam("Damage", SCRIPT_PARAM::FLOAT, &m_tStatus.Damage);
	AppendScriptParam("Health", SCRIPT_PARAM::FLOAT, &m_tStatus.curHealth);
	AppendScriptParam("Stamina", SCRIPT_PARAM::FLOAT, &m_tStatus.curStamina);
	AppendScriptParam("Defensive", SCRIPT_PARAM::FLOAT, &m_tStatus.Defensive);
	AppendScriptParam("Avoid Rate", SCRIPT_PARAM::FLOAT, &m_tStatus.AvoidPercent);
	AppendScriptParam("Critical Rate", SCRIPT_PARAM::FLOAT, &m_tStatus.CriticalPercent);
	AppendScriptParam("Critical Damage", SCRIPT_PARAM::FLOAT, &m_tStatus.CriticalDamage);
	AppendScriptParam("MoveSpeed", SCRIPT_PARAM::FLOAT, &m_tStatus.MoveSpeed);
	AppendScriptParam("AttackMoveSpeed", SCRIPT_PARAM::FLOAT, &m_tStatus.AttackMoveSpeed);
	AppendScriptParam("SpreadRatio", SCRIPT_PARAM::FLOAT, &m_tStatus.SpreadRatioSpeed);
}

void CPlayerScript::InitStateMachine()
{
	m_FSM = new CRoRStateMachine<CPlayerScript>(this, (UINT)PLAYER_STATE::END);

	FSMInit(PLAYER_STATE, CPlayerScript, NormalIdle);
	FSMInit(PLAYER_STATE, CPlayerScript, NormalReload);
	FSMInit(PLAYER_STATE, CPlayerScript, NormalAttackStart);
	FSMInit(PLAYER_STATE, CPlayerScript, NormalAttackIng);
	FSMInit(PLAYER_STATE, CPlayerScript, NormalAttackDelay);
	FSMInit(PLAYER_STATE, CPlayerScript, NormalAttackEnd);

	FSMInit(PLAYER_STATE, CPlayerScript, StandIdle);
	FSMInit(PLAYER_STATE, CPlayerScript, StandReload);
	FSMInit(PLAYER_STATE, CPlayerScript, StandAttackStart);
	FSMInit(PLAYER_STATE, CPlayerScript, StandAttackIng);
	FSMInit(PLAYER_STATE, CPlayerScript, StandAttackDelay);
	FSMInit(PLAYER_STATE, CPlayerScript, StandAttackEnd);

	FSMInit(PLAYER_STATE, CPlayerScript, KneelIdle);
	FSMInit(PLAYER_STATE, CPlayerScript, KneelReload);
	FSMInit(PLAYER_STATE, CPlayerScript, KneelAttackStart);
	FSMInit(PLAYER_STATE, CPlayerScript, KneelAttackIng);
	FSMInit(PLAYER_STATE, CPlayerScript, KneelAttackDelay);
	FSMInit(PLAYER_STATE, CPlayerScript, KneelAttackEnd);

	FSMInit(PLAYER_STATE, CPlayerScript, MoveStartNormal);
	FSMInit(PLAYER_STATE, CPlayerScript, MoveStartStand);
	FSMInit(PLAYER_STATE, CPlayerScript, MoveStartKneel);
	FSMInit(PLAYER_STATE, CPlayerScript, MoveEndNormal);
	FSMInit(PLAYER_STATE, CPlayerScript, MoveEndStand);
	FSMInit(PLAYER_STATE, CPlayerScript, MoveEndKneel);
	FSMInit(PLAYER_STATE, CPlayerScript, MoveIng);
	FSMInit(PLAYER_STATE, CPlayerScript, MoveJump);

	FSMInit(PLAYER_STATE, CPlayerScript, VitalDeath);
	FSMInit(PLAYER_STATE, CPlayerScript, VitalPanic);
	FSMInit(PLAYER_STATE, CPlayerScript, VitalDying);
	FSMInit(PLAYER_STATE, CPlayerScript, VictoryStart);
	FSMInit(PLAYER_STATE, CPlayerScript, VictoryEnd);

	FSMInit(PLAYER_STATE, CPlayerScript, SkillDash);
	FSMInit(PLAYER_STATE, CPlayerScript, SkillThrow);
	FSMInit(PLAYER_STATE, CPlayerScript, SkillCallsign);
	FSMInit(PLAYER_STATE, CPlayerScript, SkillEX);

	FSMInit(PLAYER_STATE, CPlayerScript, FormationIdle);
}

void CPlayerScript::InitSpringArmSetting()
{
	SpringArmInfo info;
	info.Type								 = true;
	info.fMaxDistance						 = 150.f;
	info.fCamSpeed							 = 20.f;
	info.fCamRotSpeed						 = 20.f;
	info.vDir								 = Vec3(0.f, 180.f, 0.f);
	info.vOffsetPos							 = Vec2(50.f, 75.f);
	m_mSpringInfos[PLAYER_STATE::NormalIdle] = info;

	info.Type										= true;
	info.fMaxDistance								= 70.f;
	info.fCamSpeed									= 30.f;
	info.fCamRotSpeed								= 20.f;
	info.vDir										= Vec3(0.f, 180.f, 0.f);
	info.vOffsetPos									= Vec2(50.f, 75.f);
	m_mSpringInfos[PLAYER_STATE::NormalAttackStart] = info;

	// info.Type									  = false;
	// info.fMaxDistance							  = 250.f;
	// info.fCamSpeed								  = 30.f;
	// info.fCamRotSpeed							  = 20.f;
	// info.vDir									  = Vec3(0.f, 0.4f, -1.f);
	// info.vOffsetPos								  = Vec2(150.f, 250.f);
	// m_mSpringInfos[PLAYER_STATE::NormalAttackEnd] = info;

	info.Type								= false;
	info.fMaxDistance						= 200.f;
	info.fCamSpeed							= 30.f;
	info.fCamRotSpeed						= 20.f;
	info.vDir								= Vec3(0.f, 180.f, 0.f);
	info.vOffsetPos							= Vec2(100.f, 75.f);
	m_mSpringInfos[PLAYER_STATE::StandIdle] = info;

	info.Type									   = false;
	info.fMaxDistance							   = 120.f;
	info.fCamSpeed								   = 30.f;
	info.fCamRotSpeed							   = 20.f;
	info.vDir									   = Vec3(0.f, 180.f, 0.f);
	info.vOffsetPos								   = Vec2(100.f, 75.f);
	m_mSpringInfos[PLAYER_STATE::StandAttackStart] = info;

	info.Type								= true;
	info.fMaxDistance						= 200.f;
	info.fCamSpeed							= 30.f;
	info.fCamRotSpeed						= 20.f;
	info.vDir								= Vec3(0.f, 180.f, 0.f);
	info.vOffsetPos							= Vec2(100.f, 75.f);
	m_mSpringInfos[PLAYER_STATE::KneelIdle] = info;

	info.Type									   = false;
	info.fMaxDistance							   = 120.f;
	info.fCamSpeed								   = 30.f;
	info.fCamRotSpeed							   = 20.f;
	info.vDir									   = Vec3(0.f, 180.f, 0.f);
	info.vOffsetPos								   = Vec2(100.f, 75.f);
	m_mSpringInfos[PLAYER_STATE::KneelAttackStart] = info;

	info.Type								= true;
	info.fMaxDistance						= 150.f;
	info.fCamSpeed							= 20.f;
	info.fCamRotSpeed						= 20.f;
	info.vDir								= Vec3(0.f, 180.f, 0.f);
	info.vOffsetPos							= Vec2(50.f, 75.f);
	m_mSpringInfos[PLAYER_STATE::SkillDash] = info;

	info.Type								 = false;
	info.fMaxDistance						 = 90.f;
	info.fCamSpeed							 = 30.f;
	info.fCamRotSpeed						 = 20.f;
	info.vDir								 = Vec3(0.f, 180.f, 0.f);
	info.vOffsetPos							 = Vec2(80.f, 40.f);
	m_mSpringInfos[PLAYER_STATE::SkillThrow] = info;

	info.Type									= false;
	info.fMaxDistance							= 100.f;
	info.fCamSpeed								= 50.f;
	info.fCamRotSpeed							= 20.f;
	info.vDir									= Vec3(0.f, 130.f, 0.f);
	info.vOffsetPos								= Vec2(0.f, 75.f);
	m_mSpringInfos[PLAYER_STATE::SkillCallsign] = info;

	info.Type							  = true;
	info.fMaxDistance					  = 150.f;
	info.fCamSpeed						  = 20.f;
	info.fCamRotSpeed					  = 20.f;
	info.vDir							  = Vec3(0.f, 160.f, 0.f);
	info.vOffsetPos						  = Vec2(50.f, 75.f);
	m_mSpringInfos[PLAYER_STATE::SkillEX] = info;

	info.Type							   = false;
	info.fMaxDistance					   = 250.f;
	info.fCamSpeed						   = 10.f;
	info.fCamRotSpeed					   = 5.f;
	info.vDir							   = Vec3(0.f, 180.f, 0.f);
	info.vOffsetPos						   = Vec2(50.f, 75.f);
	m_mSpringInfos[PLAYER_STATE::MoveJump] = info;

	info.Type								   = false;
	info.fMaxDistance						   = 150.f;
	info.fCamSpeed							   = 10.f;
	info.fCamRotSpeed						   = 5.f;
	info.vDir								   = Vec3(0.f, 0.f, 0.f);
	info.vOffsetPos							   = Vec2(0.f, 75.f);
	m_mSpringInfos[PLAYER_STATE::VictoryStart] = info;
}

#include <Engine/CRenderMgr.h>
#include <Engine\CRandomMgr.h>

bool CPlayerScript::Hit(float _damage)
{
	if (IsInvincivility())
		return false;

	m_tStatus.curHealth -= _damage;

	this->SetDamaged(true);

	if (m_tStatus.curHealth <= 0.f)
		m_tStatus.IsDead = true;

	int rndm = CRandomMgr::GetInst()->GetRandomInt(3);
	m_vecSound[(UINT)PlayerSoundType::HIT1 + rndm]->Play(1.f);

	return true;
}

int CPlayerScript::GetCurState()
{
	return m_FSM->GetCurState();
}

void CPlayerScript::SetCoverType(CoverType _type)
{
	m_iCorverType = _type;

	PLAYER_STATE state;
	switch (_type)
	{
	case CoverType::Normal:
		state = PLAYER_STATE::NormalIdle;
		break;
	case CoverType::Stand:
		state = PLAYER_STATE::MoveEndStand;
		break;
	case CoverType::Kneel:
		state = PLAYER_STATE::MoveEndKneel;
		break;
	case CoverType::End:
		break;
	default:
		break;
	}
	m_FSM->SetCurState((int)state);
}

void CPlayerScript::begin()
{
	auto vecChild = GetOwner()->GetChild();
	for (size_t i = 0; i < vecChild.size(); i++)
	{
		m_pSpringArm = vecChild[i]->GetScript<CSpringArm>();
		if (m_pSpringArm)
			break;
	}

	for (size_t i = 0; i < vecChild.size(); i++)
	{
		m_pMuzzleFlash = vecChild[i]->GetScript<CMuzzleFlashScript>();
		if (m_pMuzzleFlash)
			break;
	}

	// for (size_t i = 0; i < vecChild.size(); i++)
	//{
	//	m_pBulletLine = vecChild[i]->GetScript<CBulletLineSpawner>();
	//	if (m_pBulletLine)
	//		break;
	// }

	// 저장 재시작하면 터져서 임시로 막아둠
	if (m_pSpringArm)
	{
		m_pSpringArm->SetTargetObject(CRenderMgr::GetInst()->GetMainCam()->GetOwner());
		m_pSpringArm->SetInfo(m_mSpringInfos[PLAYER_STATE::NormalIdle]);
	}

	m_FSM->Begin();

	m_pShootingSystem = new CShootingSystemScript;

	GetOwner()->AddComponent(m_pShootingSystem);

	m_pShootingSystem->SetSpreadRatioSpeed(m_tStatus.SpreadRatioSpeed);

	auto pObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"HUD");
	if (pObj)
	{
		m_pCrosshair = pObj->GetScript<CHUD>()->GetHUD<CCrosshair>();
		m_pCrosshair->SetShootingSystem(m_pShootingSystem);
	}
	else
		CLogMgr::GetInst()->AddLog(Log_Level::ERR, L"Can't find \"HUD\"Object.");

	// Player Sound Init
	m_vecSound.resize((UINT)PlayerSoundType::End);

	Ptr<CSound> pSnd					   = CAssetMgr::GetInst()->Load<CSound>(SNDSFX_CH0138_Public_Shot);
	m_vecSound[(UINT)PlayerSoundType::EX1] = pSnd;

	pSnd								   = CAssetMgr::GetInst()->Load<CSound>(SNDSFX_Skill_Azusa_Ex_2);
	m_vecSound[(UINT)PlayerSoundType::EX2] = pSnd;

	pSnd									  = CAssetMgr::GetInst()->Load<CSound>(SNDSFX_Common_CH0240_SR_Reload_01);
	m_vecSound[(UINT)PlayerSoundType::RELOAD] = pSnd;

	pSnd										= CAssetMgr::GetInst()->Load<CSound>(SNDSFX_Field_Movement_03);
	m_vecSound[(UINT)PlayerSoundType::MOVEMENT] = pSnd;

	pSnd										= CAssetMgr::GetInst()->Load<CSound>(SNDSFX_Grenade_Throw_Up);
	m_vecSound[(UINT)PlayerSoundType::THROW_UP] = pSnd;

	pSnd										  = CAssetMgr::GetInst()->Load<CSound>(SNDSFX_Grenade_Throw_Away);
	m_vecSound[(UINT)PlayerSoundType::THROW_AWAY] = pSnd;

	pSnd									   = CAssetMgr::GetInst()->Load<CSound>(SNDSFX_Skill_Azusa_Ex_Cut_in);
	m_vecSound[(UINT)PlayerSoundType::SKILLEX] = pSnd;

	pSnd									= CAssetMgr::GetInst()->Load<CSound>(SNDAzusa_Battle_Damage_1);
	m_vecSound[(UINT)PlayerSoundType::HIT1] = pSnd;

	pSnd									= CAssetMgr::GetInst()->Load<CSound>(SNDAzusa_Battle_Damage_2);
	m_vecSound[(UINT)PlayerSoundType::HIT2] = pSnd;

	pSnd									= CAssetMgr::GetInst()->Load<CSound>(SNDAzusa_Battle_Damage_3);
	m_vecSound[(UINT)PlayerSoundType::HIT3] = pSnd;

	pSnd									   = CAssetMgr::GetInst()->Load<CSound>(SNDAzusa_Tactic_Victory_1);
	m_vecSound[(UINT)PlayerSoundType::VICTORY] = pSnd;
}

void CPlayerScript::tick()
{
	if (!m_bPlaying)
		return;

	// FSM Update,
	m_FSM->Update();
	state = magic_enum::enum_name((PLAYER_STATE)m_FSM->GetCurState());
	cover = magic_enum::enum_name((CoverType)GetCoverType());

	// 전역 무적 판정
	if (m_tStatus.Invincibility == true)
	{
		m_tStatus.fInvincibilityTimer -= DT;

		if (m_tStatus.fInvincibilityTimer <= 0.f)
		{
			SetInvincivility(false);
			m_tStatus.fInvincibilityTimer = InvincivilityTime;
		}
	}

	// 카메라 움직임
	CameraMove();

	// 노말상태 움직임
	NormalMove();
	// 노말상태 공격
	NormalAttack();

	// 조건에 따라 상태 변경해주는 함수
	ChangeToMove();
	// ChangeToNormal();
	ChangeToVictory();
	ChangeToDash();

	if (KEY_TAP(CPlayerController::Skill))
	{
		m_FSM->SetCurState((int)PLAYER_STATE::SkillThrow);
	}

	// TODO : 디버깅용 스피드 상승
	static float originSpeed = m_tStatus.MoveSpeed;
	m_tStatus.MoveSpeed		 = originSpeed;
	if (KEY_PRESSED(LSHIFT))
	{
		m_tStatus.MoveSpeed = 5 * originSpeed;
	}

	if (KEY_TAP(CPlayerController::Flip) && m_iCorverType != CoverType::Normal)
	{
		SetRight(!IsRight());
	}

	// 데미지 처리
	if (m_tStatus.IsDamaged == true)
	{
		m_FSM->SetCurState((int)PLAYER_STATE::VitalPanic);
	}

	if (KEY_PRESSED(LCTRL))
	{
		Vec3 vPos	= Transform()->GetRelativePos();
		Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

		if (KEY_TAP(F))
		{
			vPos += vFront * 50.f;
		}

		if (KEY_TAP(G))
		{
			vPos -= vFront * 50.f;
		}

		Transform()->SetRelativePos(vPos);
	}
}

void CPlayerScript::CameraMove()
{
	auto state = m_FSM->GetCurState();

	Vec3 vRot		= Transform()->GetRelativeRotation();
	Vec2 vMouseDiff = CKeyMgr::GetInst()->GetMouseDrag();

	// 캐릭터 좌우 회전 - 자식에 붙는 스프링 암도 같이 회전됨
	if (state == (int)PLAYER_STATE::NormalIdle || state == (int)PLAYER_STATE::NormalReload ||
		state == (int)PLAYER_STATE::NormalAttackStart || state == (int)PLAYER_STATE::NormalAttackDelay ||
		state == (int)PLAYER_STATE::NormalAttackIng || state == (int)PLAYER_STATE::NormalAttackEnd ||
		state == (int)PLAYER_STATE::MoveStartNormal || state == (int)PLAYER_STATE::MoveEndNormal ||
		state == (int)PLAYER_STATE::MoveIng || state == (int)PLAYER_STATE::SkillThrow)
	{
		// 마우스 x이동에 따라 y축 회전
		if (vMouseDiff.x > 0.f)
			vRot.y += CPlayerController::Sensitivity * DT;
		else if (vMouseDiff.x < 0.f)
			vRot.y -= CPlayerController::Sensitivity * DT;
		Transform()->SetRelativeRotation(vRot);
	}

	Vec3 vOffset = m_pSpringArm->GetDirOffset();

	// 카메라 상하 회전
	if (state == (int)PLAYER_STATE::NormalIdle || state == (int)PLAYER_STATE::NormalReload ||
		state == (int)PLAYER_STATE::NormalAttackStart || state == (int)PLAYER_STATE::NormalAttackDelay ||
		state == (int)PLAYER_STATE::NormalAttackIng || state == (int)PLAYER_STATE::NormalAttackEnd ||
		state == (int)PLAYER_STATE::StandIdle || state == (int)PLAYER_STATE::StandReload ||
		state == (int)PLAYER_STATE::StandAttackStart || state == (int)PLAYER_STATE::StandAttackDelay ||
		state == (int)PLAYER_STATE::StandAttackIng || state == (int)PLAYER_STATE::StandAttackEnd ||
		state == (int)PLAYER_STATE::KneelIdle || state == (int)PLAYER_STATE::KneelReload ||
		state == (int)PLAYER_STATE::KneelAttackStart || state == (int)PLAYER_STATE::KneelAttackDelay ||
		state == (int)PLAYER_STATE::KneelAttackIng || state == (int)PLAYER_STATE::KneelAttackEnd ||
		state == (int)PLAYER_STATE::MoveStartNormal || state == (int)PLAYER_STATE::MoveEndNormal ||
		state == (int)PLAYER_STATE::MoveIng || state == (int)PLAYER_STATE::MoveEndStand ||
		state == (int)PLAYER_STATE::MoveEndKneel || state == (int)PLAYER_STATE::SkillDash ||
		state == (int)PLAYER_STATE::SkillThrow)
	{
		if (m_pSpringArm && m_pSpringArm->IsActivate())
		{
			float CamRotSpeed = 10.f;
			Vec3  vRot		  = Transform()->GetRelativeRotation();

			// 마우스 y 이동에 따라 카메라 x축 회전
			{
				if (vMouseDiff.y > 0.f)
					vOffset.y += CPlayerController::Sensitivity * CamRotSpeed * DT;
				else if (vMouseDiff.y < 0.f)
					vOffset.y -= CPlayerController::Sensitivity * CamRotSpeed * DT;
				vOffset.y = clamp(vOffset.y, -20.f, 20.f);
			}

			// 마우스 y 이동에 따라 캐릭터 회전 예정 (캐릭터 발을 가리는 줌 상태인 공격 스테이트만 해당됨)
			{
				Vec3 vDiff = vRot;
				if ((state == (int)PLAYER_STATE::NormalAttackIng || state == (int)PLAYER_STATE::NormalAttackDelay ||
					 state == (int)PLAYER_STATE::NormalAttackStart || state == (int)PLAYER_STATE::StandAttackIng ||
					 state == (int)PLAYER_STATE::StandAttackDelay || state == (int)PLAYER_STATE::StandAttackStart ||
					 state == (int)PLAYER_STATE::KneelAttackStart || state == (int)PLAYER_STATE::KneelAttackIng ||
					 state == (int)PLAYER_STATE::KneelAttackDelay || state == (int)PLAYER_STATE::SkillThrow))
				{
					auto obj	= m_pSpringArm->GetTargetObject();
					auto vFront = obj->Transform()->GetWorldDir(DIR_TYPE::FRONT);

					if (vDiff.x == 0.f)
					{
						vDiff.x = -vFront.y;
					}

					CamRotSpeed = 1.f;
					if (vMouseDiff.y > 0.f)
						vDiff.x += CPlayerController::Sensitivity * CamRotSpeed * DT;
					else if (vMouseDiff.y < 0.f)
						vDiff.x -= CPlayerController::Sensitivity * CamRotSpeed * DT;
					vDiff.x	  = clamp(vDiff.x, -XM_PIDIV4, XM_PIDIV4);
					vOffset.y = 0.f;
				}
				else
				{
					vDiff.x = 0.f;
				}

				vDiff.y = vRot.y;
				vDiff.z = vRot.z;

				Transform()->SetRelativeRotation(vDiff);
			}

			// 카메라 좌우 회전
			if (state == (int)PLAYER_STATE::StandIdle || state == (int)PLAYER_STATE::StandReload ||
				state == (int)PLAYER_STATE::StandAttackStart || state == (int)PLAYER_STATE::StandAttackDelay ||
				state == (int)PLAYER_STATE::StandAttackIng || state == (int)PLAYER_STATE::StandAttackEnd ||
				state == (int)PLAYER_STATE::KneelIdle || state == (int)PLAYER_STATE::KneelReload ||
				state == (int)PLAYER_STATE::KneelAttackStart || state == (int)PLAYER_STATE::KneelAttackDelay ||
				state == (int)PLAYER_STATE::KneelAttackIng || state == (int)PLAYER_STATE::KneelAttackEnd ||
				state == (int)PLAYER_STATE::MoveEndStand || state == (int)PLAYER_STATE::MoveEndKneel ||
				state == (int)PLAYER_STATE::SkillDash)
			{
				// 마우스 x 이동에 따라 카메라 y축 회전
				CamRotSpeed = 10.f;
				if (vMouseDiff.x > 0.f)
					vOffset.x += CPlayerController::Sensitivity * CamRotSpeed * DT;
				else if (vMouseDiff.x < 0.f)
					vOffset.x -= CPlayerController::Sensitivity * CamRotSpeed * DT;

				// 대시 스킬과 그 외 상태들의 각도 범위 제한
				vOffset.x = state == (int)PLAYER_STATE::SkillDash ? clamp(vOffset.x, -45.f, 45.f)
							: IsRight()							  ? clamp(vOffset.x, -5.f, 35.f)
																  : clamp(vOffset.x, -35.f, 5.f);
			}
			else
			{
				// 카메라 좌우 회전 아닌 스테이트가 되었을 떄 카메라 y축 회전값 0으로 Lerp로 정렬
				vOffset.x = RoRMath::Lerp(vOffset.x, 0, DT);
			}
		}
	}
	if (state == (int)PLAYER_STATE::SkillEX)
		vOffset.y = RoRMath::Lerp(vOffset.y, 0, DT);

	m_pSpringArm->SetDirOffset(vOffset);
}

void CPlayerScript::NormalMove()
{
	auto state = m_FSM->GetCurState();

	// 상태 조건
	if (state == (int)PLAYER_STATE::MoveIng || state == (int)PLAYER_STATE::NormalAttackIng ||
		state == (int)PLAYER_STATE::NormalAttackStart || state == (int)PLAYER_STATE::NormalAttackDelay ||
		state == (int)PLAYER_STATE::NormalAttackEnd)
	{

		// 공격할 때 카메라 상하 회전하면서 캐릭터가 회전을 같이 하게 됨. 무지성 Front벡터를 사용할 수 없게 되고 x축
		// 회전이 0인 변환행렬 재계산이 필요함
		Vec3   vRot	   = Transform()->GetRelativeRotation();
		Matrix matRotX = XMMatrixRotationX(0.f);
		Matrix matRotY = XMMatrixRotationY(vRot.y);
		Matrix matRotZ = XMMatrixRotationZ(vRot.z);
		Matrix matRot  = matRotX * matRotY * matRotZ;

		Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		Vec3 vFront = matRot.Front();
		vFront.Normalize();
		Vec3 vPos = Transform()->GetRelativePos();

		float fMoveSpeed = state == (int)PLAYER_STATE::MoveIng ? m_tStatus.MoveSpeed : m_tStatus.AttackMoveSpeed;

		// 움직임 조건
		if (IsRight())
		{
			if (KEY_TAP(CPlayerController::Front) || KEY_PRESSED(CPlayerController::Front))
				vPos += vFront * fMoveSpeed * DT;
			if (KEY_TAP(CPlayerController::Back) || KEY_PRESSED(CPlayerController::Back))
				vPos -= vFront * fMoveSpeed * DT;
			if (KEY_TAP(CPlayerController::Right) || KEY_PRESSED(CPlayerController::Right))
				vPos += vRight * fMoveSpeed * DT;
			if (KEY_TAP(CPlayerController::Left) || KEY_PRESSED(CPlayerController::Left))
				vPos -= vRight * fMoveSpeed * DT;
		}
		else
		{
			if (KEY_TAP(CPlayerController::Front) || KEY_PRESSED(CPlayerController::Front))
				vPos += vFront * fMoveSpeed * DT;
			if (KEY_TAP(CPlayerController::Back) || KEY_PRESSED(CPlayerController::Back))
				vPos -= vFront * fMoveSpeed * DT;
			if (KEY_TAP(CPlayerController::Right) || KEY_PRESSED(CPlayerController::Right))
				vPos -= vRight * fMoveSpeed * DT;
			if (KEY_TAP(CPlayerController::Left) || KEY_PRESSED(CPlayerController::Left))
				vPos += vRight * fMoveSpeed * DT;
		}

		Transform()->SetRelativePos(vPos);
	}
}

int CPlayerScript::SwitchToCoverTypeIdle()
{
	switch (GetCoverType())
	{
	case CoverType::Normal:
		return (int)PLAYER_STATE::NormalIdle;
		break;
	case CoverType::Stand:
		return (int)PLAYER_STATE::StandIdle;
		break;
	case CoverType::Kneel:
		return (int)PLAYER_STATE::KneelIdle;
		break;
	}

	return (int)PLAYER_STATE::END;
}

int CPlayerScript::SwitchToCoverTypeMoveEnd()
{
	switch (GetCoverType())
	{
	case CoverType::Normal:
		return (int)PLAYER_STATE::MoveEndNormal;
		break;
	case CoverType::Stand:
		return (int)PLAYER_STATE::MoveEndStand;
		break;
	case CoverType::Kneel:
		return (int)PLAYER_STATE::MoveEndKneel;
		break;
	}
	return (int)PLAYER_STATE::END;
}

void CPlayerScript::ChangeToMove()
{
	auto state = m_FSM->GetCurState();

	// 탭 조건 상태들
	bool bTap = false;

	// 프레스 조건 상태들
	bool bPress = state == (int)PLAYER_STATE::NormalIdle || state == (int)PLAYER_STATE::MoveEndNormal ||
				  state == (int)PLAYER_STATE::NormalAttackEnd;

	if ((bPress && (KEY_PRESSED(CPlayerController::Front) || KEY_PRESSED(CPlayerController::Right) ||
					KEY_PRESSED(CPlayerController::Back) || KEY_PRESSED(CPlayerController::Left))) ||
		((bTap || bPress) && (KEY_TAP(CPlayerController::Front) || KEY_TAP(CPlayerController::Right) ||
							  KEY_TAP(CPlayerController::Back) || KEY_TAP(CPlayerController::Left))))
	{
		m_FSM->SetCurState((int)PLAYER_STATE::MoveIng);
	}
}

void CPlayerScript::ChangeToNormal()
{
	auto state = m_FSM->GetCurState();

	// 키 조건
	if (KEY_TAP(KEY::LSHIFT))
	{
		// 상태 조건
		if (state == (int)PLAYER_STATE::StandIdle || state == (int)PLAYER_STATE::StandReload ||
			state == (int)PLAYER_STATE::StandAttackStart || state == (int)PLAYER_STATE::StandAttackDelay ||
			state == (int)PLAYER_STATE::StandAttackIng || state == (int)PLAYER_STATE::StandAttackEnd ||
			state == (int)PLAYER_STATE::KneelIdle || state == (int)PLAYER_STATE::KneelReload ||
			state == (int)PLAYER_STATE::KneelAttackStart || state == (int)PLAYER_STATE::KneelAttackDelay ||
			state == (int)PLAYER_STATE::KneelAttackIng || state == (int)PLAYER_STATE::KneelAttackEnd ||
			state == (int)PLAYER_STATE::MoveEndStand || state == (int)PLAYER_STATE::MoveEndKneel)
		{
			m_FSM->SetCurState((int)PLAYER_STATE::NormalIdle);
		}
	}
}

void CPlayerScript::ChangeToVictory()
{
	if (KEY_TAP(Z))
	{
		m_FSM->SetCurState((int)PLAYER_STATE::VictoryStart);
	}
	// if (KEY_TAP(X))
	//{
	//	m_FSM->SetCurState((int)PLAYER_STATE::SkillCallsign);
	// }

	// if (KEY_TAP(C))
	//{
	//	m_FSM->SetCurState((int)PLAYER_STATE::SkillEX);
	// }
}

void CPlayerScript::ChangeToDash()
{
	auto state = m_FSM->GetCurState();

	if (KEY_TAP(CPlayerController::Dash))
	{
		if (state == (int)PLAYER_STATE::NormalIdle || state == (int)PLAYER_STATE::NormalReload ||
			state == (int)PLAYER_STATE::NormalAttackStart || state == (int)PLAYER_STATE::NormalAttackDelay ||
			state == (int)PLAYER_STATE::NormalAttackEnd || state == (int)PLAYER_STATE::NormalAttackIng ||
			state == (int)PLAYER_STATE::MoveIng || state == (int)PLAYER_STATE::MoveEndNormal)
		{
			m_FSM->SetCurState((int)PLAYER_STATE::SkillDash);
		}
	}
}

void CPlayerScript::NormalAttack()
{
	// 입력 조건
	if (KEY_TAP(CPlayerController::Attack) || KEY_PRESSED(CPlayerController::Attack))
	{
		auto state = m_FSM->GetCurState();

		// 상태 조건
		if (state == (int)PLAYER_STATE::NormalAttackDelay || state == (int)PLAYER_STATE::MoveIng ||
			state == (int)PLAYER_STATE::NormalIdle || state == (int)PLAYER_STATE::MoveEndNormal ||
			state == (int)PLAYER_STATE::NormalAttackStart || state == (int)PLAYER_STATE::NormalAttackEnd)

			m_FSM->SetCurState((int)PLAYER_STATE::NormalAttackIng);
	}
}

void CPlayerScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	Ptr<CMaterial> pMtrl = GetRenderComponent()->GetDynamicMaterial(0);
}

void CPlayerScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::SaveToFile(FILE* _File)
{
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
}

void CPlayerScript::SetPanicVignette()
{
	float fHpRatio	 = m_tStatus.curHealth / m_tStatus.MaxHealth * 100.f;
	int	  InputPower = 0;

	if (fHpRatio > 50.f)
		InputPower = 1;
	else if (fHpRatio < 50.f && fHpRatio > 25.f)
		InputPower = 2;
	else
		InputPower = 3;

	CRenderMgr::GetInst()->SetVignettePower(InputPower);
}

void CPlayerScript::SetPlayerCromaticAberration()
{
	float fHpRatio = m_tStatus.curHealth / m_tStatus.MaxHealth * 100.f;

	tCromatic_AberrationInfo CurEffect = {};

	if (fHpRatio > 50.f)
	{
		CRenderMgr::GetInst()->PushCAEvent(CA_TYPE::CA_SHORT);
	}
	else
	{
		CRenderMgr::GetInst()->PushCAEvent(CA_TYPE::CA_LONG);
	}
}
