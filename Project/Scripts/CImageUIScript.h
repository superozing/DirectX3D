#pragma once
#include <Engine\CUIScript.h>

class CImageUIScript : public CUIScript
{
protected:
	Ptr<CTexture> m_UIImg;
	bool		  m_bAllowBindTexPerFrame;
	bool		  m_bDraw;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetUIImg(Ptr<CTexture> _UIImg) { m_UIImg = _UIImg; }
	void BindUIImgOnTexParam();
	void Draw(bool _draw) { m_bDraw = _draw; }

	void AllowBindTexPerFrame() { m_bAllowBindTexPerFrame = true; }
	void DisallowBindTexPerFrame() { m_bAllowBindTexPerFrame = false; }

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CImageUIScript);
	CImageUIScript();
	CImageUIScript(UINT _type);
	CImageUIScript(const CImageUIScript& _Other);
	~CImageUIScript();
};
