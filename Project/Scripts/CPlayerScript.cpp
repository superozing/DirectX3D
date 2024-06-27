#include "pch.h"
#include "CPlayerScript.h"
#include <Engine/CAssetMgr.h>

#include <Engine/CGameObject.h>
#include "CMissileScript.h"

#include <Engine/CMaterial.h>
#include <Engine/CRenderComponent.h>

#include <Engine\CLogMgr.h>
#include "CRoRStateMachine.h"

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_Speed(500.f)
{
	AppendScriptParam("Player Speed", SCRIPT_PARAM::FLOAT, &m_Speed);

	// 스테이트 초기화
	m_FSM = new CRoRStateMachine<CPlayerScript>(this, (UINT)PLAYER_STATE::END);

	m_FSM->SetCallbacks((UINT)PLAYER_STATE::NORMAL, ToString(magic_enum::enum_name(PLAYER_STATE::NORMAL)),
						&CPlayerScript::NormalUpdate, &CPlayerScript::NormalBegin, &CPlayerScript::NormalEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::ATTACK, ToString(magic_enum::enum_name(PLAYER_STATE::ATTACK)),
						&CPlayerScript::AttackUpdate, &CPlayerScript::AttackBegin, &CPlayerScript::AttackEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::COVER, ToString(magic_enum::enum_name(PLAYER_STATE::COVER)),
						&CPlayerScript::CoverUpdate, &CPlayerScript::CoverBegin, &CPlayerScript::CoverEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::COVERATTACK, ToString(magic_enum::enum_name(PLAYER_STATE::COVERATTACK)),
						&CPlayerScript::CoverAttackUpdate, &CPlayerScript::CoverAttackBegin,
						&CPlayerScript::CoverAttackEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::COVERKNEEATTACK,
						ToString(magic_enum::enum_name(PLAYER_STATE::COVERKNEEATTACK)),
						&CPlayerScript::CoverKneeAttackUpdate, &CPlayerScript::CoverKneeAttackBegin,
						&CPlayerScript::CoverKneeAttackEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::DEATH, ToString(magic_enum::enum_name(PLAYER_STATE::DEATH)),
						&CPlayerScript::DeathUpdate, &CPlayerScript::DeathBegin, &CPlayerScript::DeathEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::CEREMONY, ToString(magic_enum::enum_name(PLAYER_STATE::CEREMONY)),
						&CPlayerScript::CeremonyUpdate, &CPlayerScript::CeremonyBegin, &CPlayerScript::CeremonyEnd,
						nullptr);
}

CPlayerScript::~CPlayerScript()
{
	if (nullptr != m_FSM)
	{
		delete m_FSM;
		m_FSM = nullptr;
	}
}

static string state = "";
void		  CPlayerScript::begin()
{
	m_FSM->Begin();
	AppendScriptParam("CurState", SCRIPT_PARAM::STRING, (void*)&state);
}

void CPlayerScript::tick()
{
	m_FSM->Update();
	state = magic_enum::enum_name((PLAYER_STATE)m_FSM->GetCurState());

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();

	if (KEY_PRESSED(KEY::UP))
		vPos.z += DT * m_Speed;

	if (KEY_PRESSED(KEY::DOWN))
		vPos.z -= DT * m_Speed;

	if (KEY_PRESSED(KEY::LEFT))
		vPos.x -= DT * m_Speed;

	if (KEY_PRESSED(KEY::RIGHT))
		vPos.x += DT * m_Speed;

	if (KEY_PRESSED(KEY::X))
	{
		vRot.x += DT * XM_PI;
	}

	if (KEY_PRESSED(KEY::Y))
	{
		vRot.y += DT * XM_PI;
	}

	if (KEY_PRESSED(KEY::Z))
	{
		vRot.z += DT * XM_PI;
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRot);

	if (KEY_TAP(KEY::SPACE))
	{
		Instantiate(m_Missile, Transform()->GetWorldPos(), 0);
		// GamePlayStatic::Play2DSound(L"sound\\DM.wav", 1, 0.5f, false);
		GamePlayStatic::Play2DBGM(L"sound\\DM.wav", 0.5f);
	}

	if (KEY_PRESSED(KEY::SPACE))
	{
		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial(0);
		if (nullptr != pMtrl)
		{
			pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, 1);
		}
	}
	else if (KEY_RELEASED(KEY::SPACE))
	{
		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial(0);
		if (nullptr != pMtrl)
		{
			pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, 0);
		}
	}

	// static float f = 0.f;
	// f += DT * 0.3f;
	// GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_1, f);

	// GamePlayStatic::DrawDebugRect(Vec3(0.f, 0.f, 0.f), Vec3(200.f, 200.f, 1.f), Vec3(0.f, 0.f, 0.f),
	// Vec3(1.f, 1.f, 1.f), true, 20); GamePlayStatic::DrawDebugCircle(Vec3(0.f, 0.f, 0.f), 200.f, Vec3(0.f, 1.f, 1.f),
	// true);
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
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}
