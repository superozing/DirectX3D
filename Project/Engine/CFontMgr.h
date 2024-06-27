#pragma once
#include "CManager.h"

#include <FontEngine/FW1FontWrapper.h>
#include <FontEngine/FW1CompileSettings.h>

#ifdef _DEBUG
#pragma comment(lib, "FontEngine/FW1FontWrapper_debug")
#else
#pragma comment(lib, "FontEngine/FW1FontWrapper"))
#endif

#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)

class CFontMgr : public CManager<CFontMgr>
{
	SINGLE(CFontMgr);

private:
	IFW1Factory*		m_pFW1Factory;
	IFW1FontWrapper*	m_pFontWrapper[(UINT)FONT_TYPE::END];

    vector<tFontInfo>	m_VecRenderFont;

public:
	virtual void init() override;
	virtual void enter() override {}

	void DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color, FONT_TYPE _FontType,
				  FW1_TEXT_FLAG _TextFlag = FW1_RESTORESTATE);
	void DrawFont(tFontInfo& _FontInfo);
	
	void RegisterFont(const tFontInfo& _tFontInfo) { m_VecRenderFont.push_back(_tFontInfo); }

	void render();
};
