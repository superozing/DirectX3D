﻿#pragma once

// clang-format off
// Key Value
enum KEY
{
	_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,

	NUM0, NUM1, NUM2, NUM3, NUM4,
	NUM5, NUM6, NUM7, NUM8, NUM9, 

	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,

	TAB, LSHIFT, LALT, LCTRL, SPACE,
	ESC, ENTER, BACK, DEL,
	LEFT, RIGHT, UP, DOWN,

	LBTN, RBTN, MBTN,

	F1, F2, F3, F4, F5, F6,
	F7, F8, F9, F10, F11, F12,

	KEY_END,
};

enum WHEEL_STATE {
	WHEEL_NONE,
	WHEEL_UP,
	WHEEL_DOWN,
};

enum KEY_STATE
{
	NONE,
	TAP,
	PRESSED,
	RELEASED,
};

struct FKeyData
{
	KEY			eKey;
	KEY_STATE	eState;
	bool		bPressed;
};
// clang-format on

typedef FOCUS_STATE (*FOCUS_CALLBACK)(void);

class CKeyMgr : public CManager<CKeyMgr>
{
	SINGLE(CKeyMgr);

private:
	vector<FKeyData> m_vecKeyData;

	Vec2 m_vMousePos;
	Vec2 m_vMousePrevPos;

	Vec2 m_vMouseDrag;

	Vec2 m_vMouseOffset;

	WHEEL_STATE _inWheel;
	WHEEL_STATE m_ThisFrameWheel;

	FOCUS_STATE m_FocusState;

	bool m_bHoldMouseCenter;
	bool m_bShowCursor = true;

public:
	FOCUS_CALLBACK m_FocusCallback;

public:
	void RoRShowCursor(bool _show);
	bool IsShowCursor() { return m_bShowCursor; }

public:
	KEY_STATE GetKeyState(KEY _Key)
	{
		if (_Key == KEY::KEY_END)
			return NONE;
		return m_vecKeyData[_Key].eState;
	}
	Vec2 GetMousePos() const { return m_vMousePos; }
	Vec2 GetMouseDrag() const
	{
		if (m_FocusState == FOCUS_STATE::MAIN)
			return m_vMouseDrag;
		return Vec2();
	}
	Vec2 GetMouseDrag_Editor() const
	{
		if (m_FocusState != FOCUS_STATE::MAIN)
			return m_vMouseDrag;
		return Vec2();
	}

	// 마우스 현재 위치를 계산할 때 Offset을 줍니다.
	void AddMouseOffset(Vec2 _MouseOffset) { m_vMouseOffset += _MouseOffset; }
	void SetMouseOffset(Vec2 _MouseOffset) { m_vMouseOffset = _MouseOffset; }
	void ClearMouseOffset() { m_vMouseOffset = Vec2(); }

	Vec2 GetMouseOffset() const { return m_vMouseOffset; }

	void		SetWheel(WHEEL_STATE _in) { _inWheel = _in; }
	WHEEL_STATE GetWheel() { return m_ThisFrameWheel; }

	FOCUS_STATE GetFocusState() const { return m_FocusState; }
	void		SetFocuseState(FOCUS_STATE _state) { m_FocusState = _state; }

	void SetHoldMouseCenter(bool _bHoldMouseCenter) { m_bHoldMouseCenter = _bHoldMouseCenter; }

public:
	virtual void init() override;
	virtual void tick() override;
	virtual void enter() override {}
};
