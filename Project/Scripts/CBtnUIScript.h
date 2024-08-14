#pragma once
#include <Engine/CUIScript.h>

typedef void (*BtnCallBack)(void);
typedef void (CEntity::*DelegateFunc)(void);

enum class BtnUISoundType
{
	Click,
	Hover,
	End,
};

enum class ButtonState
{
	Normal,
	Hovered,
	Pressed,
	END,
};

class CBtnUIScript : public CUIScript
{
private:
	bool		m_bDraw;
	ButtonState m_State;

	Ptr<CTexture> m_NormalImg;
	Vec2		  m_vNLT;
	Vec2		  m_vNSz;
	Vec4		  m_vNColor;
	Ptr<CTexture> m_HoverImg;
	Vec2		  m_vHLT;
	Vec2		  m_vHSz;
	Vec4		  m_vHColor;
	Ptr<CTexture> m_PressedImg;
	Vec2		  m_vPLT;
	Vec2		  m_vPSz;
	Vec4		  m_vPColor;
	Ptr<CTexture> m_CurImg;
	Vec2		  m_vCLT;
	Vec2		  m_vCSz;
	Vec4		  m_vCColor;

	bool m_AllowTexSet;
	bool m_AllowCallFunc = true;

	// 콜백 (전역함수 포인터)
	BtnCallBack m_CallBackFunc;

	// 델리게이트(맴버함수 포인터, 객체)
	CEntity*	 m_Inst;
	DelegateFunc m_Delegate;

	vector<Ptr<CSound>> m_vecSound;

public:
	ButtonState GetState() { return m_State; }

	void Draw(bool _draw) { m_bDraw = _draw; }
	bool IsDraw() { return m_bDraw; }

public:
	void SetNormalImg(Ptr<CTexture> _NormalImg) { m_NormalImg = _NormalImg; }
	void SetPressedImg(Ptr<CTexture> _PressedImg) { m_PressedImg = _PressedImg; }
	void SetHoverImg(Ptr<CTexture> _HoverImg) { m_HoverImg = _HoverImg; }

	void AllowTexSet() { m_AllowTexSet = true; }
	void DisallowTexSet() { m_AllowTexSet = false; }

	void AllowCallFunc() { m_AllowCallFunc = true; }
	void DisallowCallFunc() { m_AllowCallFunc = false; }

	void SetCallBack(BtnCallBack _CallBack) { m_CallBackFunc = _CallBack; }

	// SetDeletage((CEntity*)객체, (DelegateFunc)&클래스명::함수명);
	void SetDeletage(CEntity* _Inst, DelegateFunc _Func)
	{
		m_Inst	   = _Inst;
		m_Delegate = _Func;
	}

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	virtual void OnHovered() override;
	virtual void OnUnHovered() override;
	virtual void MouseOn() override;

	virtual void LBtnDown() override;
	virtual void LBtnUp() override;
	virtual void LBtnClicked() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CBtnUIScript);
	CBtnUIScript();
	CBtnUIScript(const CBtnUIScript& _Other);
	~CBtnUIScript();
};