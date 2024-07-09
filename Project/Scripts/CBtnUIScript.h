#pragma once
#include <Engine/CUIScript.h>

typedef void (*BtnCallBack)(void);
typedef void (CEntity::*DelegateFunc)(void);

class CBtnUIScript : public CUIScript
{
private:
	Ptr<CTexture> m_NormalImg;
	Ptr<CTexture> m_HoverImg;
	Ptr<CTexture> m_PressedImg;
	Ptr<CTexture> m_CurImg;

	bool m_AllowTexSet;
	bool m_AllowCallFunc = true;

	// 콜백 (전역함수 포인터)
	BtnCallBack m_CallBackFunc;

	// 델리게이트(맴버함수 포인터, 객체)
	CEntity*	 m_Inst;
	DelegateFunc m_Delegate;

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