#pragma once
#include <Engine\CUIScript.h>

class CTextUI : public CUIScript
{
private:
	bool	  m_bDraw;
	string	  m_strText;
	tFontInfo m_tInfo;
	Vec4	  m_vColor;

public:
	bool IsDraw() { return m_bDraw; }
	void Draw(bool _draw) { m_bDraw = _draw; }

public:
	void init();

	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CTextUI);
	CTextUI();
	CTextUI(const CTextUI& _Origin);
	CTextUI(UINT _type);
	~CTextUI();
};
