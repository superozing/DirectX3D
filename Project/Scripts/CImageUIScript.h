#pragma once
#include <Engine\CUIScript.h>

class CImageUIScript : public CUIScript
{
protected:
	Ptr<CTexture> m_UIImg;
	bool		  m_bAllowBindTexPerFrame;
	bool		  m_bDraw;
	Vec4		  m_vBlendColor;

public:
	void		 init();
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetColor(Vec4 _color) { m_vBlendColor = _color; }
	Vec4 GetColor() { return m_vBlendColor; }

	void SetUIImg(Ptr<CTexture> _UIImg) { m_UIImg = _UIImg; }
	void BindUIImgOnTexParam();
	void Draw(bool _draw) { m_bDraw = _draw; }
	bool IsDraw() { return m_bDraw; }

	void AllowBindTexPerFrame() { m_bAllowBindTexPerFrame = true; }
	void DisallowBindTexPerFrame() { m_bAllowBindTexPerFrame = false; }

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CImageUIScript);
	CImageUIScript();
	CImageUIScript(UINT _type);
	CImageUIScript(const CImageUIScript& _Other);
	~CImageUIScript();
};
