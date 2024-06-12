#pragma once
#include "CManager.h"

class CUIScript;

class CUIMgr :
	public CManager<CUIMgr>
{
private:
	CGameObject* m_FocusedObject; // 최상위 부모 UI 중에서 포커싱 되어있는 UI
	CUIScript* m_FocusedUI;	 // 최상위 부모 UI 중에서 포커싱 되어있는 UI

public:
	virtual void tick() override;
	virtual void enter() override {}

private:
	CUIScript* GetPriorityCheck(CUIScript* _ParentUI);

};