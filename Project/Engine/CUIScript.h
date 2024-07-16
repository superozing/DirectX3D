﻿#pragma once
#include "CScript.h"

class CUIScript : public CScript
{
private:
	Vec2 m_vFinalPos;

	bool m_bMouseCheckAble = true;

	bool m_bMouseOn;
	bool m_bMouseOn_Prev;

	bool m_bMouseLBtnDown;

protected:
	UI_TYPE m_UIType;

public:
	virtual void tick() override;

	virtual void OnHovered() {}
	virtual void MouseOn() {}
	virtual void OnUnHovered() {}

	virtual void LBtnDown() {}
	virtual void LBtnUp() {}
	virtual void LBtnClicked() {}

public:
	void EnableMouseInput() { m_bMouseCheckAble = true; }
	void DisableMouseInput() { m_bMouseCheckAble = false; }

	Vec2 GetFinalPos() const { return m_vFinalPos; }
	bool IsLBtnDown() const { return m_bMouseLBtnDown; }

	bool IsMouseInputEnabled() const { return m_bMouseCheckAble; }

	UI_TYPE GetUIType() const { return m_UIType; }
	void	SetUIType(UI_TYPE _type) { m_UIType = _type; }

protected:
	CUIScript(UINT _ScriptType);

public:
	CLONE(CUIScript);
	CUIScript();
	CUIScript(const CUIScript& _Other);
	~CUIScript();

	friend class CUIMgr;
};