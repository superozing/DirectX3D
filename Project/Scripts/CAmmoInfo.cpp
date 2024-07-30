#include "pch.h"
#include "CAmmoInfo.h"

#include <Engine/CFontMgr.h>
#include <Engine/CDevice.h>

CAmmoInfo::CAmmoInfo()
	: CTextUI((UINT)SCRIPT_TYPE::AMMOINFO)
{
	AppendScriptParam("CurAmmo", SCRIPT_PARAM::INT, &m_iCurAmmo);
	AppendScriptParam("MaxAmmo", SCRIPT_PARAM::INT, &m_iMaxAmmo);
}

CAmmoInfo::CAmmoInfo(const CAmmoInfo& _Origin)
	: CTextUI((UINT)SCRIPT_TYPE::AMMOINFO)
{
	AppendScriptParam("CurAmmo", SCRIPT_PARAM::INT, &m_iCurAmmo);
	AppendScriptParam("MaxAmmo", SCRIPT_PARAM::INT, &m_iMaxAmmo);
}

CAmmoInfo::~CAmmoInfo()
{
}

void CAmmoInfo::begin()
{
	const auto& childs = GetOwner()->GetChild();
	for (const auto& child : childs)
	{
	}

	// TODO : 사격 시스템 생기면 포인터 받아야함,
	// curammo랑 maxammo랑 연결해주어야 함
}

void CAmmoInfo::tick()
{
}

void CAmmoInfo::SaveToFile(ofstream& fout)
{
}

void CAmmoInfo::LoadFromFile(ifstream& fin)
{
}
