#pragma once
#include <Engine\CUIScript.h>

class CTextUI : public CUIScript
{
private:
	bool	m_bDraw;
	string	m_strText;
	bool	m_bWstring;
	wstring m_wstrText;

	tFontInfo m_tInfo;
	Vec4	  m_vColor;

public:
	void SetColor(Vec4 _Color);
	Vec4 GetColor() { return m_vColor; }

	bool IsDraw() { return m_bDraw; }
	void Draw(bool _draw) { m_bDraw = _draw; }
	void SetText(const string& _str)
	{
		m_bWstring = false;
		m_strText  = _str;
	}
	void SetText(const wstring& _str)
	{
		m_bWstring = true;
		m_wstrText = _str;
	}

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
