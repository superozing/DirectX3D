#pragma once
#include <Engine/CScript.h>

class CBtnUIScript;
class CPausePanel;

class CPauseBtn : public CScript
{
private:
	CBtnUIScript* m_pPauseBtnUI;
	CPausePanel*  m_pPausePanel;

	Ptr<CTexture> m_PauseBtnTex;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	void		  SetPausePanel(CPausePanel* _pPausePanel) { m_pPausePanel = _pPausePanel; }
	void		  SetPauseBtnTex(Ptr<CTexture> _PauseBtnTex) { m_PauseBtnTex = _PauseBtnTex; }
	Ptr<CTexture> GetPauseBtnTex() { return m_PauseBtnTex; }

public:
	CLONE(CPauseBtn);
	CPauseBtn();
	~CPauseBtn();
};
