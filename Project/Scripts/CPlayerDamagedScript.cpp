#include "pch.h"
#include "CPlayerDamagedScript.h"

#include <Engine/CTimeMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>

CPlayerDamagedScript::CPlayerDamagedScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERDAMAGEDSCRIPT)
	, m_Vignette(nullptr)
	, m_IsDamaged(false)
	, m_VignetteType(0)
	, m_VignetteAlpha(0.f)
	, m_Duration(0.f)
{
	AppendScriptParam("IsDamaged   ", SCRIPT_PARAM::BOOL, &m_IsDamaged);
	AppendScriptParam("VignetteType", SCRIPT_PARAM::INT, &m_VignetteType, 0, 4);
}

CPlayerDamagedScript::~CPlayerDamagedScript()
{
}

void CPlayerDamagedScript::begin()
{
	m_Vignette = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"VignetteTest");
}

void CPlayerDamagedScript::tick()
{
	m_Duration += DT;
	m_Vignette->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, m_VignetteType);

	if (m_IsDamaged)
	{
		m_VignetteAlpha = 0.8f;
		m_Vignette->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_VignetteAlpha);
		m_IsDamaged = false;
		m_Duration	= 0.f;
	}

	if (m_VignetteAlpha > 0.f && m_Duration > 1.f)
	{
		m_VignetteAlpha -= DT * 0.8f;

		if (m_VignetteAlpha <= 0.f)
			m_VignetteAlpha = 0.f;

		m_Vignette->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_VignetteAlpha);
	}
}
