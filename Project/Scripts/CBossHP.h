#pragma once
#include "CProgressBar.h"

class CBossHP : public CProgressBar
{
private:
	// 초상화
	CImageUIScript* m_pPortrait;
	Ptr<CTexture>	m_PortraitTex;

	// 폰트 텍스쳐
	CImageUIScript* m_pImgFont;
	Ptr<CTexture>	m_ImgFontTex;

	// 바 별 체력
	int m_LineHP;

	// HP Line UI
	CImageUIScript* m_pHPLineUI;


public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetPortraitTex(Ptr<CTexture> _PortraitTex);
	void SetImgFontTex(Ptr<CTexture> _ImgFontTex);

	void SetMaxHP(int _MaxHP) { CProgressBar::SetMaxHP(RoRMath::ClampInt(_MaxHP, 0)); }
	void SetLineHP(int _LineHP);
	void SetCurHP(int _CurHP) { CProgressBar::SetCurHP(RoRMath::ClampInt(_CurHP, 0, CProgressBar::GetMaxHP())); }

	int GetMaxHP() const { return CProgressBar::GetMaxHP(); }
	int GetLineHP() const { return m_LineHP; }
	int GetCurHP() const { return CProgressBar::GetCurHP(); }

public:
	// 디버그용 함수
	void Add100();
	void Sub100();

private:
	virtual void MakeChildObjects() override;

public:
	CLONE(CBossHP);
	CBossHP();
	~CBossHP();
};
