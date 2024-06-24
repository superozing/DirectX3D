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
	m_FSM = new CRoRStateMachine<CPlayerScript>(this, (UINT)PLAYER_STATE::END);

	m_FSM->SetCallbacks((UINT)PLAYER_STATE::NORMAL, ToString(magic_enum::enum_name(PLAYER_STATE::NORMAL)),
						&CPlayerScript::NormalUpdate, &CPlayerScript::NormalBegin, &CPlayerScript::NormalEnd, nullptr);
	m_FSM->SetCallbacks((UINT)PLAYER_STATE::ATTACK, ToString(magic_enum::enum_name(PLAYER_STATE::ATTACK)),
						&CPlayerScript::AttackUpdate, &CPlayerScript::AttackBegin, &CPlayerScript::AttackEnd, nullptr);
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
void CPlayerScript::begin()
{
	m_FSM->Begin();
	AppendScriptParam("CurState", SCRIPT_PARAM::STRING, (void *)&state);
	//Ptr<CTexture> pAltasTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\link.png", L"texture\\link.png");
	//Animator2D()->Create(L"IDLE_LEFT", pAltasTex, Vec2(0.f, 130.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 3, 10);
	//Animator2D()->Create(L"IDLE_RIGHT", pAltasTex, Vec2(0.f, 390.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 3, 10);

	//Animator2D()->Create(L"MOVE_UP", pAltasTex, Vec2(0.f, 780.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 10, 20);
	//Animator2D()->Create(L"MOVE_DOWN", pAltasTex, Vec2(0.f, 520.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 10, 20);
	//Animator2D()->Create(L"MOVE_LEFT", pAltasTex, Vec2(0.f, 650.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 10, 20);
	//Animator2D()->Create(L"MOVE_RIGHT", pAltasTex, Vec2(0.f, 910.f), Vec2(120.f, 130.f), Vec2(0.f, 0.f), Vec2(200.f, 200.f), 10, 20);

	//GetRenderComponent()->GetDynamicMaterial(0);

	//m_Missile = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"MissilePrefab");
	//m_Missile = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\missile.pref", L"prefab\\missile.pref");
}

void CPlayerScript::tick()
{
	m_FSM->Update();
	state = magic_enum::enum_name((PLAYER_STATE)m_FSM->GetCurState());

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();

	if (KEY_PRESSED(KEY::UP))
		vPos.z += DT * m_Speed;	
	//if (KEY_TAP(KEY::UP))
	//	Animator2D()->Play(L"MOVE_UP");
	//if (KEY_RELEASED(UP))
	//	Animator2D()->Play(L"IDLE_UP");
	
	if (KEY_PRESSED(KEY::DOWN))
		vPos.z -= DT * m_Speed;
	//if (KEY_TAP(KEY::DOWN))
	//	Animator2D()->Play(L"MOVE_DOWN");
	//if (KEY_RELEASED(DOWN))
	//	Animator2D()->Play(L"IDLE_DOWN");

	if (KEY_PRESSED(KEY::LEFT))
		vPos.x -= DT * m_Speed;
	//if (KEY_TAP(KEY::LEFT))
	//	Animator2D()->Play(L"MOVE_LEFT");
	//if (KEY_RELEASED(LEFT))
	//	Animator2D()->Play(L"IDLE_LEFT");

	if (KEY_PRESSED(KEY::RIGHT))
		vPos.x += DT * m_Speed;
	//if (KEY_TAP(KEY::RIGHT))
	//	Animator2D()->Play(L"MOVE_RIGHT");
	//if (KEY_RELEASED(RIGHT))
	//	Animator2D()->Play(L"IDLE_RIGHT");

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
		//GamePlayStatic::Play2DSound(L"sound\\DM.wav", 1, 0.5f, false);
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

	//static float f = 0.f;
	//f += DT * 0.3f;
	//GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_1, f);

	//GamePlayStatic::DrawDebugRect(Vec3(0.f, 0.f, 0.f), Vec3(200.f, 200.f, 1.f), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), true, 20);
	//GamePlayStatic::DrawDebugCircle(Vec3(0.f, 0.f, 0.f), 200.f, Vec3(0.f, 1.f, 1.f), true);


}

void CPlayerScript::BeginOverlap(CCollider2D* _Collider
	, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
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

void CPlayerScript::NormalBegin()
{
}

int CPlayerScript::NormalUpdate()
{
	if (KEY_TAP(KEY::C))
	{
		return (UINT)PLAYER_STATE::ATTACK;
	}
	else
	{
		return (UINT)PLAYER_STATE::NORMAL;
	}
}

void CPlayerScript::NormalEnd()
{
}

static float Att_Acctime = 0.f;
static const float Att_duration = 1.f;

void CPlayerScript::AttackBegin()
{
	Att_Acctime = 0.f;
}

int CPlayerScript::AttackUpdate()
{
	Att_Acctime += DT;
	if (Att_duration < Att_Acctime)
	{
		return (UINT)PLAYER_STATE::NORMAL;
	}
	else
	{
		return (UINT)PLAYER_STATE::ATTACK;
	}
}

void CPlayerScript::AttackEnd()
{
}