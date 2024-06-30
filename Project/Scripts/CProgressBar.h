#pragma once
#include <Engine\CScript.h>

class CImageUIScript;
class CPanelUIScript;

// 파생되는 자식 클래스가 해야하는 일.
// 
// 1. GetPanelUI()로 paneltex와 여러 옵션 set
// 2. MaxHP, CurHP 조정
// 3. MakeChildObjects() override, 
//		이후 HPLineTex 등 자식 오브젝트로 추가
class CProgressBar : public CScript
{
private:
	// 메인 패널UI
	// 자식 쪽에서 텍스쳐 등을 전부 Set 해주어야 해요.
	CPanelUIScript* m_pPanelUI;

	// 최대 체력
	int m_MaxHP;

	// 현재 체력
	int m_CurHP;

protected:
	virtual void	MakeChildObjects() {}
	CPanelUIScript* GetPanelUI() { return m_pPanelUI; }

	void SetMaxHP(int _MaxHP) { m_MaxHP = _MaxHP; }
	void SetCurHP(int _CurHP) { m_CurHP = _CurHP; }

	int GetMaxHP() const { return m_MaxHP; }
	int GetCurHP() const { return m_CurHP; }

public:
	virtual void begin() override;
	virtual void tick() override {}

public:
	CLONE(CProgressBar);
	CProgressBar();
	CProgressBar(UINT _ScriptType);
	~CProgressBar();
};
