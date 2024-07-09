#pragma once
#include <Engine\CScript.h>

class CImageUIScript;
class CPanelUIScript;
class CBtnUIScript;

class CPausePanel : public CScript
{
private:
	CGameObject*	m_pPausePanelObject;
	CPanelUIScript* m_pPausePanel;	// 일시 정지 상태 시 패널 이미지
	tFontInfo		m_PanelFontInfo;

	CImageUIScript* m_pModalBg;		// 모달 시 바탕 이미지

	CBtnUIScript*	m_pExitBtn;		// 나가기 버튼

	CBtnUIScript*	m_pFuncBtn[2];	// 기능 버튼
	tFontInfo		m_FuncBtnFontInfo[2]; // 기능 버튼에 그려질 폰트 정보

	bool			m_bActivate; // 활성화 여부

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void ActivePausePanel();
	void InactivePausePanel();

public:
	CLONE(CPausePanel);
	CPausePanel();
	~CPausePanel();
};
