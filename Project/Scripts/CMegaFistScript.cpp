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

	m_CurState = m_Shooter->GetScript<CBossScript>()->GetBossFSM()->GetCurState();

	shooter = ToString(m_Shooter->GetName());
	target	= ToString(m_Target->GetName());

	Transform()->SetRelativePos(m_Pos);
	Transform()->SetDir(-Transform()->GetWorldDir(DIR_TYPE::UP));
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

	Vec3 vPos		= Transform()->GetRelativePos();
	Vec3 vTargetPos = m_Target->Transform()->GetRelativePos();
	Vec3 vDir		= vTargetPos - vPos;
	Transform()->SetDir(vDir);

	Vec3 vFront = vDir;
	vFront.Normalize();
	vPos += vFront * m_InitialSpeed * DT;
	Transform()->SetRelativePos(vPos);
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
}
