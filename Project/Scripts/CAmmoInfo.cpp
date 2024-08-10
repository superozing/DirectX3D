#include "pch.h"
#include "CAmmoInfo.h"

#include <Engine/CFontMgr.h>
#include <Engine/CDevice.h>

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include "CPlayerScript.h"
#include "CShootingSystemScript.h"

#include "CDigitUI.h"

CAmmoInfo::CAmmoInfo()
	: CTextUI((UINT)SCRIPT_TYPE::AMMOINFO)
{
}

CAmmoInfo::CAmmoInfo(const CAmmoInfo& _Origin)
	: CTextUI((UINT)SCRIPT_TYPE::AMMOINFO)
{
}

CAmmoInfo::~CAmmoInfo()
{
}

void CAmmoInfo::begin()
{
	const auto& childs = GetOwner()->GetChild();
	for (const auto& child : childs)
	{
		auto script = child->GetScript<CDigitUI>();
		if (script)
		{
			if (child->GetName() == L"CurAmmo")
				m_pCurAmmo = script;

			if (child->GetName() == L"MaxAmmo")
				m_pMaxAmmo = script;
		}
	}

	m_pSS = CLevelMgr::GetInst()
				->GetCurrentLevel()
				->FindObjectByName(PlayerName)
				->GetScript<CPlayerScript>()
				->GetShootingSystem();

	m_pCurAmmo->SetNum(m_pSS->GetCurAmmo());
	m_pMaxAmmo->SetNum(m_pSS->GetMaxAmmo());
}

void CAmmoInfo::tick()
{
	CTextUI::tick();
	m_pCurAmmo->SetNum(m_pSS->GetCurAmmo());
}

void CAmmoInfo::SaveToFile(ofstream& fout)
{
	CTextUI::SaveToFile(fout);
}

void CAmmoInfo::LoadFromFile(ifstream& fin)
{
	CTextUI::LoadFromFile(fin);
}
