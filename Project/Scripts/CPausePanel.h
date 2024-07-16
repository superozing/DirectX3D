#pragma once
#include <Engine\CScript.h>

class CImageUIScript;
class CPanelUIScript;
class CBtnUIScript;

class CPausePanel : public CScript
{
private:
	// 부모 패널UI
	CPanelUIScript* m_pPanelUI; // 일시 정지 상태 시 패널 이미지
	tFontInfo		m_PanelFontInfo;

	CImageUIScript* m_pModalBg; // 모달 시 바탕 이미지 (회색)

	CBtnUIScript* m_pContinueBtn; // 계속하기 버튼
	CBtnUIScript* m_pExitBtn;	  // 나가기 버튼

	bool m_bActivate; // 활성화 여부

	bool m_tempActiveUIType[(UINT)UI_TYPE::END];

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	void ActivePausePanel();
	void InactivePausePanel();

	void ExitBtnDelegateFunc() {}

private:
	void SetParentPanelUI();

public:
	CLONE(CPausePanel);
	CPausePanel();
	~CPausePanel();
};
