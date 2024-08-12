#pragma once
#include "CProgressBar.h"

class CBossHP : public CProgressBar
{
private:
	class CBossScript* m_pBossScript;

	// 초상화
	CImageUIScript* m_pPortrait;

	// 폰트 텍스쳐
	CImageUIScript* m_pImgFont;

	// HP Line UI
	CImageUIScript* m_pHPLineUI;

	// Boss Groggy Gauge
	CImageUIScript* m_pGroggyGaugeUI;

	// 바 별 체력
	int m_LineHP;

	int m_CurLerpHP;

	tFontInfo m_HPFont;
	tFontInfo m_BossNameFont;
	tFontInfo m_LineCountFont;

	class CBossLV* m_GameMode;
	class CTextUI* m_BossHPText;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	void SetBossScript(CBossScript* _pBossScript) { m_pBossScript = _pBossScript; }

	void SetMaxHP(int _MaxHP)
	{
		CProgressBar::SetMaxValue(RoRMath::ClampInt(_MaxHP, 0));
		m_CurLerpHP = _MaxHP;
	}
	void SetLineHP(int _LineHP);
	void SetCurHP(int _CurHP) { CProgressBar::SetCurValue(RoRMath::ClampInt(_CurHP, 0, CProgressBar::GetMaxValue())); }

	int GetMaxHP() const { return CProgressBar::GetMaxValue(); }
	int GetLineHP() const { return m_LineHP; }
	int GetCurHP() const { return CProgressBar::GetCurValue(); }

public:
	// 디버그용 함수
	void Add100();
	void Sub100();

	// 음수를 입력할 경우 체력을 회복합니다.
	void AppendDamage(int _Damage);

	void DbgAppendDamageWrap();

private:
	virtual void MakeChildObjects() override;

public:
	CLONE(CBossHP);
	CBossHP();
	~CBossHP();
};
