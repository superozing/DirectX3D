#pragma once
#include "CRenderComponent.h"

class CFontRender : public CRenderComponent
{
private:
	tFontInfo	m_FontInfo;
	Vec3		m_FontOffset;

	bool		m_AllowConvertWorldPosToWindowPos;

public:
	void SetWString(const wstring& _str)		{ m_FontInfo.WStr = _str; }
	void SetFont(const FONT_TYPE& _FontType)	{ m_FontInfo.FontType = _FontType; }
	void SetFontSize(float _Size)				{ m_FontInfo.fFontSize = _Size; }
	void SetOffsetPos(const Vec3& _vPos)		{ m_FontOffset = _vPos; }
	void SetFlags(const FW1_TEXT_FLAG& _flags)	{ m_FontInfo.TextFlag = _flags; }
	// Use FONT_RGBA() Macro Function.
	void SetFontColor(UINT _FONT_RGBA)	{ m_FontInfo.Color = _FONT_RGBA; }
	void SetFontInfo(const tFontInfo& _FontInfo) { m_FontInfo = _FontInfo; }

	void AllowConvertWorldPosToWindowPos(bool _allow) { m_AllowConvertWorldPosToWindowPos = _allow; }

	const wstring&			GetWString() const { return m_FontInfo.WStr; }
	const FONT_TYPE&		GetFont() const { return m_FontInfo.FontType; }
	float					GetFontSize() const { return m_FontInfo.fFontSize; }
	const Vec3&				GetOffsetPos() const { return m_FontOffset; }
	const FW1_TEXT_FLAG&	GetFlags() const { return m_FontInfo.TextFlag; }
	const tFontInfo&		GetFontInfo() { return m_FontInfo; }


private:
	Vec3 ConvertUIWorldPosToWindowPos(Vec3 _WorldPos);

public:
	virtual void UpdateData() override {}
	virtual void finaltick() override;
	virtual void render() override;

public:
	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

public:
	CLONE(CFontRender);
	CFontRender();
	~CFontRender();
};
