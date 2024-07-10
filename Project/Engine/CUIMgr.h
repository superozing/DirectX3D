#pragma once
#include "CManager.h"

class CUIScript;

class CUIMgr : public CManager<CUIMgr>
{
private:
	CGameObject* m_FocusedObject; // 최상위 부모 UI 중에서 포커싱 되어있는 UI
	CUIScript*	 m_FocusedUI;	  // 최상위 부모 UI 중에서 포커싱 되어있는 UI

	// debug - viewport 상의 마우스 좌표
	// release - main window 상의 마우스 좌표
	Vec2 m_vWorldMousePos;

	// ui active 정보를 담은 배열
	bool m_arrUIActive[(UINT)UI_TYPE::END];

public:
	virtual void init() override;

	virtual void tick() override;
	virtual void enter() override {}

public:
	// debug - viewport 상의 마우스 좌표를 반환
	// release - main window 상의 마우스 좌표를 반환
	Vec2 GetWorldMousePos() const { return m_vWorldMousePos; }

	// 해당 UI 타입이 활성화 된 상태인지 반환
	bool IsActiveUIType(UI_TYPE _type) const { return m_arrUIActive[(UINT)_type]; }
	void ActiveUIType(UI_TYPE _type) { m_arrUIActive[(UINT)_type] = true; }
	void InactiveUIType(UI_TYPE _type) { m_arrUIActive[(UINT)_type] = false; }

private:
	CUIScript* GetPriorityCheck(CGameObject* _Parent);

	void CalWorldMousePos();
};