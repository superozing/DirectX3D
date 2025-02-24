﻿#include "pch.h"
#include "CBossSwordTrailScript.h"

#include "CBossScript.h"

CBossSwordTrailScript::CBossSwordTrailScript()
	: CScript((UINT)SCRIPT_TYPE::BOSSSWORDTRAILSCRIPT)
	, m_Kaiten(nullptr)
	, m_TrailOffset(-0.9f)
{
}

CBossSwordTrailScript::~CBossSwordTrailScript()
{
}

void CBossSwordTrailScript::begin()
{
	if (GetOwner()->GetParent())
		m_Kaiten = GetOwner()->GetParent();

	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_TrailOffset);

	m_Kaiten->GetScript<CBossScript>()->GetSound((UINT)BOSS_SOUND::EX4_BLADING)->Play(1.f, 2.f, true);
}

void CBossSwordTrailScript::tick()
{
	if (m_Kaiten->GetScript<CBossScript>()->IsVital())
	{
		GamePlayStatic::DestroyGameObject(GetOwner());
	}

	int LayerIdx = m_Kaiten->Animator3D()->GetCurFrameIdx();

	if (LayerIdx > 120 && LayerIdx < 135)
	{
		m_TrailOffset += DT * 1.5f;

		if (m_TrailOffset >= 0.8f)
			m_TrailOffset = 0.8f;
	}
	else if (LayerIdx > 135)
	{
		m_TrailOffset = -0.9f;

		if (LayerIdx > 140)
			GamePlayStatic::DestroyGameObject(GetOwner());
	}

	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_TrailOffset);
}
