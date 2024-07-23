#include "pch.h"
#include "CPlayerScript.h"
#include <Engine/CAssetMgr.h>

#include <Engine/CGameObject.h>
#include "CMissileScript.h"

#include <Engine/CMaterial.h>
#include <Engine/CRenderComponent.h>

#include <Engine/CKeyMgr.h>
#include <Engine/CPhysXMgr.h>
#include <Engine/CLogMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CRandomMgr.h>

#include "CRoRStateMachine.h"
#include "CPlayerController.h"
#include "CSpringArm.h"

#include "CCrosshair.h"
#include "CDamagedDirectionMgr.h"

static string state = "";
static string cover = "";

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_tStatus{}
	, m_pSpringArm(nullptr)
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
	AppendScriptParam("Damage", SCRIPT_PARAM::FLOAT, &m_tStatus.Damage);
	AppendScriptParam("Health", SCRIPT_PARAM::FLOAT, &m_tStatus.curHealth);
	AppendScriptParam("Stamina", SCRIPT_PARAM::FLOAT, &m_tStatus.curStamina);
	AppendScriptParam("Defensive", SCRIPT_PARAM::FLOAT, &m_tStatus.Defensive);
	AppendScriptParam("Avoid Rate", SCRIPT_PARAM::FLOAT, &m_tStatus.AvoidPercent);
	AppendScriptParam("Critical Rate", SCRIPT_PARAM::FLOAT, &m_tStatus.CriticalPercent);
	AppendScriptParam("Critical Damage", SCRIPT_PARAM::FLOAT, &m_tStatus.CriticalDamage);
	AppendScriptParam("MoveSpeed", SCRIPT_PARAM::FLOAT, &m_tStatus.MoveSpeed);
	AppendScriptParam("AttackMoveSpeed", SCRIPT_PARAM::FLOAT, &m_tStatus.AttackMoveSpeed);
	AppendScriptParam("SpreadRatio", SCRIPT_PARAM::FLOAT, &m_tStatus.SpreadRatio);
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
	info.fCamSpeed							 = 40.f;
	info.fCamRotSpeed						 = 20.f;
	info.vDir								 = Vec3(-20.f, 180.f, 0.f);
	info.vOffsetPos							 = Vec2(50.f, 50.f);
	m_mSpringInfos[PLAYER_STATE::NormalIdle] = info;

	info.Type										= false;
	info.fMaxDistance								= 70.f;
	info.fCamSpeed									= 30.f;
	info.fCamRotSpeed								= 20.f;
	info.vDir										= Vec3(-20.f, 180.f, 0.f);
	info.vOffsetPos									= Vec2(50.f, 50.f);
	m_mSpringInfos[PLAYER_STATE::NormalAttackStart] = info;

	// info.Type									  = false;
	// info.fMaxDistance							  = 250.f;
	// info.fCamSpeed								  = 30.f;
	// info.fCamRotSpeed							  = 20.f;
	// info.vDir									  = Vec3(0.f, 0.4f, -1.f);
	// info.vOffsetPos								  = Vec2(150.f, 250.f);
	// m_mSpringInfos[PLAYER_STATE::NormalAttackEnd] = info;

	info.Type								= false;
	info.fMaxDistance						= 150.f;
	info.fCamSpeed							= 30.f;
	info.fCamRotSpeed						= 20.f;
	info.vDir								= Vec3(-20.f, 180.f, 0.f);
	info.vOffsetPos							= Vec2(100.f, 50.f);
	m_mSpringInfos[PLAYER_STATE::StandIdle] = info;

	info.Type									   = false;
	info.fMaxDistance							   = 70.f;
	info.fCamSpeed								   = 30.f;
	info.fCamRotSpeed							   = 20.f;
	info.vDir									   = Vec3(-20.f, 180.f, 0.f);
	info.vOffsetPos								   = Vec2(100.f, 50.f);
	m_mSpringInfos[PLAYER_STATE::StandAttackStart] = info;

	info.Type								= true;
	info.fMaxDistance						= 150.f;
	info.fCamSpeed							= 30.f;
	info.fCamRotSpeed						= 20.f;
	info.vDir								= Vec3(-20.f, 180.f, 0.f);
	info.vOffsetPos							= Vec2(100.f, 50.f);
	m_mSpringInfos[PLAYER_STATE::KneelIdle] = info;

	info.Type									   = false;
	info.fMaxDistance							   = 70.f;
	info.fCamSpeed								   = 30.f;
	info.fCamRotSpeed							   = 20.f;
	info.vDir									   = Vec3(-20.f, 180.f, 0.f);
	info.vOffsetPos								   = Vec2(100.f, 50.f);
	m_mSpringInfos[PLAYER_STATE::KneelAttackStart] = info;

	info.Type								= true;
	info.fMaxDistance						= 150.f;
	info.fCamSpeed							= 20.f;
	info.fCamRotSpeed						= 20.f;
	info.vDir								= Vec3(-20.f, 180.f, 0.f);
	info.vOffsetPos							= Vec2(50.f, 50.f);
	m_mSpringInfos[PLAYER_STATE::SkillDash] = info;

	info.Type								 = false;
	info.fMaxDistance						 = 90.f;
	info.fCamSpeed							 = 30.f;
	info.fCamRotSpeed						 = 20.f;
	info.vDir								 = Vec3(-20.f, 180.f, 0.f);
	info.vOffsetPos							 = Vec2(80.f, 40.f);
	m_mSpringInfos[PLAYER_STATE::SkillThrow] = info;

	info.Type									= false;
	info.fMaxDistance							= 100.f;
	info.fCamSpeed								= 50.f;
	info.fCamRotSpeed							= 20.f;
	info.vDir									= Vec3(-30.f, 130.f, 0.f);
	info.vOffsetPos								= Vec2(0.f, 50.f);
	m_mSpringInfos[PLAYER_STATE::SkillCallsign] = info;

	info.Type							  = true;
	info.fMaxDistance					  = 150.f;
	info.fCamSpeed						  = 20.f;
	info.fCamRotSpeed					  = 20.f;
	info.vDir							  = Vec3(-20.f, 160.f, 0.f);
	info.vOffsetPos						  = Vec2(50.f, 50.f);
	m_mSpringInfos[PLAYER_STATE::SkillEX] = info;

	info.Type							   = false;
	info.fMaxDistance					   = 250.f;
	info.fCamSpeed						   = 10.f;
	info.fCamRotSpeed					   = 5.f;
	info.vDir							   = Vec3(-20.f, 180.f, 0.f);
	info.vOffsetPos						   = Vec2(50.f, 50.f);
	m_mSpringInfos[PLAYER_STATE::MoveJump] = info;

	info.Type								   = false;
	info.fMaxDistance						   = 150.f;
	info.fCamSpeed							   = 10.f;
	info.fCamRotSpeed						   = 5.f;
	info.vDir								   = Vec3(-10.f, 0.f, 0.f);
	info.vOffsetPos							   = Vec2(0.f, 50.f);
	m_mSpringInfos[PLAYER_STATE::VictoryStart] = info;
}

#include <Engine/CRenderMgr.h>

void CPlayerScript::begin()
{
	auto vecChild = GetOwner()->GetChild();
	for (size_t i = 0; i < vecChild.size(); i++)
	{
		m_pSpringArm = vecChild[i]->GetScript<CSpringArm>();
		if (m_pSpringArm)
			break;
	}

	// 저장 재시작하면 터져서 임시로 막아둠
	if (m_pSpringArm)
		m_pSpringArm->SetTargetObject(CRenderMgr::GetInst()->GetMainCam()->GetOwner());

	m_FSM->Begin();

	auto pObj	 = new CGameObject;
	m_pCrosshair = new CCrosshair;
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pCrosshair);
	pObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
	pObj->SetName(L"Crosshair");
	GamePlayStatic::SpawnGameObject(pObj, (UINT)LAYER::LAYER_UI);

	m_pDamagedDirectionMgr = new CDamagedDirectionMgr;
	GetOwner()->AddComponent(m_pDamagedDirectionMgr);
}

void CPlayerScript::tick()
{
	// FSM Update,
	m_FSM->Update();
	state = magic_enum::enum_name((PLAYER_STATE)m_FSM->GetCurState());
	cover = magic_enum::enum_name((CoverType)GetCoverType());

	// 카메라 움직임
	CameraMove();

	// 노말상태 움직임
	NormalMove();
	// 노말상태 공격
	NormalAttack();

	// 조건에 따라 상태 변경해주는 함수
	ChangeToMove();
	ChangeToNormal();
	ChangeToVictory();
	ChangeToDash();

	// 엄폐 판정 할 수 있게되면 지울 함수
	SwitchCoverType();

	if (KEY_TAP(CPlayerController::Skill))
	{
		m_FSM->SetCurState((int)PLAYER_STATE::SkillThrow);
	}

	// 일반 Raycast
	// int mask = RayCastDebugFlag::EndPointVisible;

	tRoRHitInfo hitInfo	 = {};
	auto		pMainCam = CRenderMgr::GetInst()->GetMainCam();
	auto		FrontDir = pMainCam->Transform()->GetWorldDir(DIR_TYPE::FRONT);

	bool isContact = CPhysXMgr::GetInst()->PerfomRaycast(pMainCam->Transform()->GetWorldPos(), FrontDir, hitInfo,
														 (UINT)LAYER::LAYER_RAYCAST, RayCastDebugFlag::AllVisible);

	if (isContact)
	{
		// m_tStatus.SpreadRatio = RoRMath::ClampFloat(m_tStatus.SpreadRatio + 0.7f * DT, 0.f, 1.f);
		m_pCrosshair->SetCrosshairColor(Vec4(255.f, 0.f, 0.f, 255.f));
	}
	else
	{
		// m_tStatus.SpreadRatio = RoRMath::ClampFloat(m_tStatus.SpreadRatio - 0.7f * DT, 0.f, 1.f);
		m_pCrosshair->SetCrosshairColor(Vec4(255.f, 255.f, 255.f, 255.f));
	}

	m_pCrosshair->SetSpreadRatio(m_tStatus.SpreadRatio);

	hitInfo			= {};
	float MaxSpread = 0.3f;

	float RotX = m_tStatus.SpreadRatio * CRandomMgr::GetInst()->GetRandomFloat() * MaxSpread;
	float RotY = m_tStatus.SpreadRatio * CRandomMgr::GetInst()->GetRandomFloat() * MaxSpread;

	FrontDir.x += RotX;
	FrontDir.y += RotY;

	bool isBulletHit = CPhysXMgr::GetInst()->PerfomRaycast(pMainCam->Transform()->GetWorldPos(), FrontDir, hitInfo,
														   (UINT)LAYER::LAYER_RAYCAST, RayCastDebugFlag::AllVisible);

	if (isBulletHit)
	{
		// 데미지 처리, 데칼 오브젝트 추가 등등...
	}

	m_pCrosshair->SetSpreadRatio(m_tStatus.SpreadRatio);

	// 임시로 키 입력 시 피격 효과 추가
	if (KEY_TAP(KEY::B))
	{
		auto pMon = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Temp Monster Cube");
		m_pDamagedDirectionMgr->AddDamagedDirection(pMon->Transform()->GetWorldPos(), 0.1f);
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
				vOffset.y = clamp(vOffset.y, -20.f, 5.f);
			}

			// 마우스 y 이동에 따라 캐릭터 회전 예정 (캐릭터 발을 가리는 줌 상태인 공격 스테이트만 해당됨)
			{
				Vec3 vDiff = Vec3(vOffset.y, vOffset.x, vOffset.z);
				vDiff.ToRadian();
				vDiff *= 3.f;
				if (!(state == (int)PLAYER_STATE::NormalAttackIng || state == (int)PLAYER_STATE::NormalAttackDelay ||
					  state == (int)PLAYER_STATE::NormalAttackStart || state == (int)PLAYER_STATE::StandAttackIng ||
					  state == (int)PLAYER_STATE::StandAttackDelay || state == (int)PLAYER_STATE::StandAttackStart ||
					  state == (int)PLAYER_STATE::KneelAttackStart || state == (int)PLAYER_STATE::KneelAttackIng ||
					  state == (int)PLAYER_STATE::KneelAttackDelay || state == (int)PLAYER_STATE::SkillThrow))
					vDiff.x = 0.f;

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
				if (vMouseDiff.x > 0.f)
					vOffset.x += CPlayerController::Sensitivity * CamRotSpeed * DT;
				else if (vMouseDiff.x < 0.f)
					vOffset.x -= CPlayerController::Sensitivity * CamRotSpeed * DT;

				// 대시 스킬과 그 외 상태들의 각도 범위 제한
				vOffset.x =
					state == (int)PLAYER_STATE::SkillDash ? clamp(vOffset.x, -45.f, 45.f) : clamp(vOffset.x, 0.f, 45.f);
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
		if (KEY_TAP(CPlayerController::Front) || KEY_PRESSED(CPlayerController::Front))
			vPos += vFront * fMoveSpeed * DT;
		if (KEY_TAP(CPlayerController::Back) || KEY_PRESSED(CPlayerController::Back))
			vPos -= vFront * fMoveSpeed * DT;
		if (KEY_TAP(CPlayerController::Right) || KEY_PRESSED(CPlayerController::Right))
			vPos += vRight * fMoveSpeed * DT;
		if (KEY_TAP(CPlayerController::Left) || KEY_PRESSED(CPlayerController::Left))
			vPos -= vRight * fMoveSpeed * DT;

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
	if (KEY_TAP(X))
	{
		m_FSM->SetCurState((int)PLAYER_STATE::SkillCallsign);
	}

	if (KEY_TAP(C))
	{
		m_FSM->SetCurState((int)PLAYER_STATE::SkillEX);
	}
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

void CPlayerScript::SwitchCoverType()
{
	if (KEY_TAP(KEY::_1))
		SetCoverType(CoverType::Normal);
	if (KEY_TAP(KEY::_2))
		SetCoverType(CoverType::Stand);
	if (KEY_TAP(KEY::_3))
		SetCoverType(CoverType::Kneel);
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
