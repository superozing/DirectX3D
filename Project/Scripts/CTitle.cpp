#include "pch.h"
#include "CTitle.h"

#include "CTitleTex.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

CTitle::CTitle()
	: CScript((UINT)SCRIPT_TYPE::TITLE)
{
}

CTitle::~CTitle()
{
}

void CTitle::DrawRemnants()
{
	Vec4 vColor = m_pRemnants->GetColor();

	float speed = 5.f;

	if (vColor.w >= 1.f)
	{
		vColor.w			 = 1.f;
		m_bRemnantsDarkStart = true;
	}

	if (vColor.w < 0.f)
	{
		m_pRemnants->Draw(false);
		m_bRemnantsEnd = true;
		return;
	}

	if (m_bRemnantsDarkStart)
		speed *= -1.f;

	vColor.w += DT / speed;
	m_pRemnants->SetColor(vColor);
}

void CTitle::RemnantsBrightStart()
{
	m_pRemnants->Draw(true);
	Vec4 vColor = m_pRemnants->GetColor();
	vColor.w	= 0.f;
	m_pRemnants->SetColor(vColor);
}

void CTitle::RemnantsDarkStart()
{
	Vec4 vColor			 = m_pRemnants->GetColor();
	vColor.w			 = 1.f;
	m_bRemnantsDarkStart = true;
	m_pRemnants->SetColor(vColor);
}

void CTitle::DrawBlueArchive()
{
	if (!m_bBlueArchiveBrightStart)
	{
		BlueArchiveBrightStart();
	}

	Vec4 vColor = m_pBlueArchive->GetColor();

	float speed = 5.f;

	if (vColor.w >= 1.f)
	{
		vColor.w				= 1.f;
		m_bBlueArchiveDarkStart = true;
	}

	if (vColor.w < 0.f)
	{
		m_pBlueArchive->Draw(false);
		m_bBlueArchiveEnd = true;
		return;
	}

	if (m_bBlueArchiveDarkStart)
		speed *= -1.f;

	vColor.w += DT / speed;
	m_pBlueArchive->SetColor(vColor);
}

void CTitle::BlueArchiveBrightStart()
{
	m_pRemnants->Draw(false);
	m_pBlueArchive->Draw(true);
	m_bRemnantsEnd			  = true;
	m_bBlueArchiveBrightStart = true;

	Vec4 vColor = m_pBlueArchive->GetColor();
	vColor.w	= 0.f;
	m_pBlueArchive->SetColor(vColor);
}

void CTitle::BlueArchiveDarkStart()
{
	m_bBlueArchiveDarkStart = true;

	Vec4 vColor = m_pBlueArchive->GetColor();
	vColor.w	= 1.f;
	m_pBlueArchive->SetColor(vColor);
}

void CTitle::SwitchToNextTexture()
{
	if (!m_bRemnantsDarkStart)
	{
		RemnantsDarkStart();
		return;
	}

	if (!m_bRemnantsEnd)
	{
		BlueArchiveBrightStart();
		return;
	}

	if (!m_bBlueArchiveDarkStart)
	{
		BlueArchiveDarkStart();
		return;
	}

	if (!m_bTitleShow)
	{
		m_bTitleShow = true;
		auto pLevel	 = CLevelMgr::GetInst()->LevelLoadFunc(LEVELSelectMenu);
		GamePlayStatic::ChangeLevel(pLevel, LEVEL_STATE::PLAY);
		return;
	}
}

void CTitle::begin()
{
	const auto& childs = GetOwner()->GetChild();
	for (auto& child : childs)
	{
		auto script = child->GetScript<CTitleTex>();
		if (script)
		{
			if (child->GetName() == L"Remnants")
			{
				m_pRemnants = script;
			}
			if (child->GetName() == L"BlueArchive")
			{
				m_pBlueArchive = script;
			}
		}
	}
	m_pRemnants->Draw(false);
	m_pBlueArchive->Draw(false);
	RemnantsBrightStart();
}

void CTitle::tick()
{
	if (!m_bTitleShow)
	{
		if (!m_bRemnantsEnd)
		{
			DrawRemnants();
		}

		if (m_bRemnantsEnd && !m_bBlueArchiveEnd)
		{
			DrawBlueArchive();
		}

		if (KEY_TAP(LBTN))
		{
			SwitchToNextTexture();
		}
	}
}