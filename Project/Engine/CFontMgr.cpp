#include "pch.h"
#include "CFontMgr.h"

#include "CDevice.h"

CFontMgr::CFontMgr()
	: m_pFW1Factory(nullptr)
	, m_pFontWrapper{}
{
}

CFontMgr::~CFontMgr()
{
	if (nullptr != m_pFW1Factory)
		m_pFW1Factory->Release();

	for (auto& it : m_pFontWrapper)
		if (nullptr != it)
			it->Release();
}

void CFontMgr::init()
{
	// FW1Factory
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1Factory)))
		assert(NULL);

	// FontWrapper

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Arial", &m_pFontWrapper[(UINT)FONT_TYPE::ARIAL])))
		assert(NULL);

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Maplestory", &m_pFontWrapper[(UINT)FONT_TYPE::MAPLE])))
		assert(NULL);
}

void CFontMgr::DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color,
						FONT_TYPE _FontType, FW1_TEXT_FLAG _TextFlag)
{
	m_pFontWrapper[(UINT)_FontType]->DrawString(
							   CONTEXT,
							   _pStr,			// String
							   _fFontSize,		// Font size
							   _fPosX,			// X position
							   _fPosY,			// Y position
							   _Color,			// Text color, 0xAaBbGgRr
							   _TextFlag   // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void CFontMgr::render()
{
	// 여기서 뎁스 소팅

	for (auto& it : m_VecRenderFont)
		DrawFont(it.WStr.c_str(), it.fPosX, it.fPosY, it.fFontSize, it.Color, it.FontType, it.TextFlag);

	m_VecRenderFont.clear();
}
