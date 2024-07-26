#include "pch.h"
#include "CAmmoInfo.h"

#include <Engine/CFontMgr.h>
#include <Engine/CDevice.h>

CAmmoInfo::CAmmoInfo()
	: CUIScript((UINT)SCRIPT_TYPE::AMMOINFO)
	, m_bDraw(true)
{
	AppendScriptParam("Draw", SCRIPT_PARAM::BOOL, &m_bDraw);
}

CAmmoInfo::CAmmoInfo(const CAmmoInfo& _Origin)
	: CUIScript((UINT)SCRIPT_TYPE::AMMOINFO)
	, m_bDraw(_Origin.m_bDraw)
{
	AppendScriptParam("Draw", SCRIPT_PARAM::BOOL, &m_bDraw);
}

CAmmoInfo::~CAmmoInfo()
{
}

void CAmmoInfo::begin()
{
}

void CAmmoInfo::tick()
{
	// UI가 활성화 상태일 경우에만 폰트 등록
	if (m_bDraw)
	{
		// 폰트의 색상 설정
		m_tAmmoFont.Color = FONT_RGBA(255, 255, 255, 255);

		// 폰트의 위치 설정

		Vec3 vWorldPos = Transform()->GetWorldPos();
		Vec2 vResol	   = CDevice::GetInst()->GetRenderResolution();

		m_tAmmoFont.vPos = Vec2(vWorldPos.x + (vResol.x / 2), -vWorldPos.y + (vResol.y / 2));

		m_tAmmoFont.WStr = L"TEST";

		CFontMgr::GetInst()->RegisterFont(m_tAmmoFont);
	}
}

void CAmmoInfo::SaveToFile(ofstream& fout)
{
}

void CAmmoInfo::LoadFromFile(ifstream& fin)
{
}
