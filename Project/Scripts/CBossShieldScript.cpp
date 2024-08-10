#include "pch.h"
#include "CBossShieldScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include "CBossScript.h"

CBossShieldScript::CBossShieldScript()
	: CScript((UINT)SCRIPT_TYPE::BOSSSHIELDSCRIPT)
	, m_Parent(nullptr)
	, m_Triangle(nullptr)
	, m_ShieldTime(0.f)
	, m_ShieldType(SHIELD_TYPE::END)
{
	AppendScriptParam("ShieldTime", SCRIPT_PARAM::FLOAT, &m_ShieldTime, 0.f, 1.f);
}

CBossShieldScript::~CBossShieldScript()
{
}

void CBossShieldScript::begin()
{
	m_Parent->AddChild(GetOwner());

	auto child = GetOwner()->GetChild();
	if (!child.empty())
	{
		m_Triangle = child[0];
	}
}

void CBossShieldScript::tick()
{
	if (!m_Parent)
		return;

	if (m_Parent->GetScript<CBossScript>()->IsVital())
	{
		GamePlayStatic::DestroyGameObject(GetOwner());
	}

	if (SHIELD_TYPE::InnerShield == m_ShieldType)
		CheckInnerShield();
	else if (SHIELD_TYPE::OutsideShield == m_ShieldType)
		CheckOutsideShield();
	else if (SHIELD_TYPE::HexShield == m_ShieldType)
		CheckHexShield();
}

void CBossShieldScript::CheckInnerShield()
{
	int	  idx	= m_Parent->Animator3D()->GetCurFrameIdx();
	float ftime = DT * 2.f;

	if (36 <= idx && idx <= 149)
		m_ShieldTime += ftime;
	else if (idx > 149)
		m_ShieldTime -= ftime;

	if (m_ShieldTime >= 1.f)
		m_ShieldTime = 1.f;
	else if (m_ShieldTime <= 0.f)
		m_ShieldTime = 0.f;

	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_ShieldTime);

	if (idx > 149 && m_ShieldTime <= 0.f)
		GamePlayStatic::DestroyGameObject(GetOwner());
}

void CBossShieldScript::CheckOutsideShield()
{
	int	  idx	= m_Parent->Animator3D()->GetCurFrameIdx();
	float ftime = DT * 3.f;

	if (46 <= idx && idx <= 145)
		m_ShieldTime += ftime;
	else if (idx > 145)
		m_ShieldTime -= ftime;

	if (m_ShieldTime >= 1.f)
		m_ShieldTime = 1.f;
	else if (m_ShieldTime <= 0.f)
		m_ShieldTime = 0.f;

	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_ShieldTime);
	m_Triangle->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_ShieldTime);
	m_Triangle->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, m_ShieldTime);

	if (idx > 145 && m_ShieldTime <= 0.f)
		GamePlayStatic::DestroyGameObject(GetOwner());
}

void CBossShieldScript::CheckHexShield()
{
	int	  idx	= m_Parent->Animator3D()->GetCurFrameIdx();
	float ftime = DT;

	if (idx > 30 && idx < 155)
		m_ShieldTime += ftime;
	else if (idx > 155)
		m_ShieldTime -= ftime;

	if (m_ShieldTime >= 1.f)
		m_ShieldTime = 1.f;
	else if (m_ShieldTime <= 0.f)
		m_ShieldTime = 0.f;

	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_ShieldTime);
}
