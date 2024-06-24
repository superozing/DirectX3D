#pragma once
#include "CManager.h"

class CUIScript;

class CUIMgr :
	public CManager<CUIMgr>
{
private:
	CGameObject* m_FocusedObject; // 최상위 부모 UI 중에서 포커싱 되어있는 UI
	CUIScript* m_FocusedUI;	 // 최상위 부모 UI 중에서 포커싱 되어있는 UI

	// debug - viewport 상의 마우스 좌표
	// release - main window 상의 마우스 좌표
	Vec2 m_vWorldMousePos;

public:
	virtual void tick() override;
	virtual void enter() override {}

public:

	// debug - viewport 상의 마우스 좌표를 반환
	// release - main window 상의 마우스 좌표를 반환
	Vec2 GetWorldMousePos() const { return m_vWorldMousePos; }

private:
	CUIScript* GetPriorityCheck(CUIScript* _ParentUI);
	
	void CalWorldMousePos();
};