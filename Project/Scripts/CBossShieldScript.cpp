#include "pch.h"
#include "CBossShieldScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

CBossShieldScript::CBossShieldScript()
	: CScript((UINT)SCRIPT_TYPE::BOSSSHIELDSCRIPT)
	, m_Parent(nullptr)
	, m_ShieldTime(0.f)
{
	AppendScriptParam("ShieldTime", SCRIPT_PARAM::FLOAT, &m_ShieldTime, 0.f, 1.f);
}

CBossShieldScript::~CBossShieldScript()
{
}

void CBossShieldScript::begin()
{
	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_ShieldTime);

	m_Parent->AddChild(GetOwner());
}

void CBossShieldScript::tick()
{
	if (!m_Parent)
		return;

	int	  idx	= m_Parent->Animator3D()->GetCurFrameIdx();
	float ftime = DT * 1.5f;

	if (36 <= idx && idx <= 149)
		m_ShieldTime += ftime;
	else if (idx > 149)
		m_ShieldTime -= ftime;

	if (m_ShieldTime >= 1.f)
		m_ShieldTime = 1.f;
	else if (m_ShieldTime <= 0.f)
		m_ShieldTime = 0.f;

	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_ShieldTime);

	if (idx > 149 && m_ShieldTime <= 0.f)
		GamePlayStatic::DestroyGameObject(GetOwner());
}
