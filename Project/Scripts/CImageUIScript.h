#pragma once
#include <Engine\CUIScript.h>

class CImageUIScript : public CUIScript
{
private:
	Ptr<CTexture> m_UIImg;
	bool		  m_bAllowBindTexPerFrame;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetUIImg(Ptr<CTexture> _UIImg) { m_UIImg = _UIImg; }
	void BindUIImgOnTexParam();

	void AllowBindTexPerFrame() { m_bAllowBindTexPerFrame = true; }
	void DisallowBindTexPerFrame() { m_bAllowBindTexPerFrame = false; }

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CImageUIScript);
	CImageUIScript();
	CImageUIScript(const CImageUIScript& _Other);
	~CImageUIScript();
};
