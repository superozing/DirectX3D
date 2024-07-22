#include "pch.h"
#include "CMegaFistScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>

#include "CBossScript.h"
#include "CRoRStateMachine.h"

static string shooter = "";
static string target  = "";

CMegaFistScript::CMegaFistScript()
	: CProjectileScript((UINT)SCRIPT_TYPE::MEGAFISTSCRIPT)
	, m_Shooter(nullptr)
	, m_Target(nullptr)
	, m_CurState((int)BOSS_STATE::END)
	, m_TargetPos(0.f, 0.f, 0.f)
	, m_Direction(0.f, 0.f, 0.f)
	, m_Gravity(0.f, -9.81f, 0.f)
	, m_FistSpeed(0.f, 0.f, 0.f)
{
	AppendScriptParam("Shooter", SCRIPT_PARAM::STRING, &shooter);
	AppendScriptParam("Target ", SCRIPT_PARAM::STRING, &target);
}

CMegaFistScript::~CMegaFistScript()
{
}

void CMegaFistScript::begin()
{
	CProjectileScript::begin();

	shooter = ToString(m_Shooter->GetName());
	target	= ToString(m_Target->GetName());

	m_CurState = m_Shooter->GetScript<CBossScript>()->GetBossFSM()->GetCurState();

	Transform()->SetRelativePos(m_Pos);

	m_Direction	  = m_TargetPos - m_Pos;
	m_Direction.y = m_Pos.y;
	m_Direction.Normalize();
	m_FistSpeed = m_Direction * m_InitialSpeed;

	m_FistSpeed += m_Gravity;

	Transform()->SetDir(m_FistSpeed);
}

void CMegaFistScript::tick()
{
	CProjectileScript::tick();

	if (m_IsAlive)
	{
		m_LifeSpan -= DT;

		if (m_LifeSpan <= 0.f)
		{
			m_IsAlive = false;
			// GamePlayStatic::DestroyGameObject(GetOwner());
		}
	}

	CalVelocity();

	// m_FistSpeed += m_Gravity;
	// Transform()->SetDir(m_FistSpeed);

	m_Pos		= Transform()->GetRelativePos();
	Vec3 vFront = m_FistSpeed;
	vFront.Normalize();
	m_Pos += vFront * m_InitialSpeed * DT;
	Transform()->SetRelativePos(m_Pos);

	// Vec3 vPos = Transform()->GetRelativePos();
	// Vec3 vDir = m_TargetPos - vPos;
	// Transform()->SetDir(vDir);

	// Vec3 vFront = vDir;
	// vFront.Normalize();
	// vPos += vFront * m_InitialSpeed * DT;
	// Transform()->SetRelativePos(vPos);
}

void CMegaFistScript::OnHit()
{
}

void CMegaFistScript::InitMegaFistInfo(CGameObject* _Shooter, CGameObject* _Target, Vec3 _Pos, float _InitSpeed,
									   float _MaxSpeed, float _LifeSpan, float _Damage, bool _Explode, bool _Alive)
{
	m_Shooter = _Shooter;
	m_Target  = _Target;
	CProjectileScript::InitProjectileInfo(_Pos, _InitSpeed, _MaxSpeed, _LifeSpan, _Damage, _Explode, _Alive);

	m_TargetPos = m_Target->Transform()->GetRelativePos();

	m_Direction = m_TargetPos - m_Pos;
}

void CMegaFistScript::CalVelocity()
{
	m_Direction = m_TargetPos - m_Pos;
	m_Direction.Normalize();
	m_FistSpeed = m_Direction * m_InitialSpeed;

	m_FistSpeed += m_Gravity * DT;

	Transform()->SetDir(m_FistSpeed);
}
