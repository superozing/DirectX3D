#include "pch.h"
#include "CTextUI.h"

#include <Engine\CFontMgr.h>
#include <Engine\CDevice.h>

CTextUI::CTextUI()
	: CUIScript((UINT)SCRIPT_TYPE::TEXTUI)
	, m_vColor(Vec4(255.f, 255.f, 255.f, 255.f))
	, m_bDraw(true)

{
	init();
}

CTextUI::CTextUI(const CTextUI& _Origin)
	: CUIScript(_Origin)
	, m_vColor(Vec4(255.f, 255.f, 255.f, 255.f))
	, m_bDraw(true)
	, m_strText(_Origin.m_strText)
{
	m_tInfo.fFontSize = _Origin.m_tInfo.fFontSize;
}

CTextUI::CTextUI(UINT _type)
	: CUIScript(_type)
	, m_vColor(Vec4(255.f, 255.f, 255.f, 255.f))
	, m_bDraw(true)
{
	init();
}

CTextUI::~CTextUI()
{
}

void CTextUI::init()
{
	AppendScriptParam("Draw", SCRIPT_PARAM::BOOL, &m_bDraw);
	AppendScriptParam("FontColor", SCRIPT_PARAM::VEC4, &m_vColor);
	AppendScriptParam("FontSize", SCRIPT_PARAM::FLOAT, &m_tInfo.fFontSize);
	AppendScriptParam("FontContent", SCRIPT_PARAM::STRING, &m_strText);
	m_tInfo.fFontSize = 32.f;
}

void CTextUI::begin()
{
}

void CTextUI::tick()
{
	// UI가 활성화 상태일 경우에만 폰트 등록
	if (IsDraw())
	{
		// 폰트의 색상 설정
		m_tInfo.Color = FONT_RGBA(m_vColor.x, m_vColor.y, m_vColor.z, m_vColor.w);

		// 폰트의 위치 설정

		Vec3 vWorldPos = Transform()->GetWorldPos();
		Vec2 vResol	   = CDevice::GetInst()->GetRenderResolution();

		m_tInfo.vPos = Vec2(vWorldPos.x + (vResol.x / 2), -vWorldPos.y + (vResol.y / 2));

		m_tInfo.WStr = ToWString(m_strText);

		CFontMgr::GetInst()->RegisterFont(m_tInfo);
	}
}

#define TagColor "[FontColor]"
#define TagSize "[FontSize]"
#define TagText "[ContentText]"

void CTextUI::SaveToFile(ofstream& fout)
{
	fout << TagColor << endl;
	fout << m_vColor << endl;

	fout << TagSize << endl;
	fout << m_tInfo.fFontSize << endl;

	fout << TagText << endl;
	fout << ToString(m_strText) << endl;
}

void CTextUI::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagColor);
	fin >> m_vColor;

	Utils::GetLineUntilString(fin, TagSize);
	fin >> m_tInfo.fFontSize;

	Utils::GetLineUntilString(fin, TagText);
	getline(fin, m_strText);
}