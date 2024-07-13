#pragma once
#include <Engine/CScript.h>

class CUIScript;
class CImageUIScript;
class CPanelUIScript;

class CDialog : public CScript
{
	// 대화 창에 필요한 것.
	// 1. 캐릭터 이미지
	// 2. 배경 이미지
	// 3. 대화 배경
	// 4. 폰트
	//	1) 이름
	//	2) 대화 내용
	//	3) 구분선
	// 전부 UI 레이어, UI 카메라 소속
private:
	// 부모 패널UI
	CPanelUIScript* m_pOwnerPanelUI;

	// 캐릭터 이미지
	CImageUIScript* m_pCharatorUI;
	Ptr<CTexture>	m_pCharatorTex;

	// 배경 이미지
	CImageUIScript* m_pCharatorBgUI;
	Ptr<CTexture>	m_pBgTex;

	// 폰트 배경 (쉐이더 코드를 사용해서 배경 표시 용도)
	CImageUIScript* m_pFontBgUI;

	// 구분선
	CImageUIScript* m_pDividingLineUI;
	// 이건 그냥 쉐이더 코드 써서 간단하게 끼워 넣기만 해도 될 것 같은데?

	// 캐릭터 이름
	tFontInfo m_CharactorNameFontInfo;

	// 대화 내용
	tFontInfo m_DialogFontInfo;

public:
	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

	virtual void begin() override;
	virtual void tick() override;

public:
	// Get, Set 함수
	void SetCharatorName(wstring& _CharatorName) { m_CharactorNameFontInfo.WStr = _CharatorName; }
	void SetDialogWString(wstring& _DialogWString) { m_DialogFontInfo.WStr = _DialogWString; }

	void SetCharatorTex(Ptr<CTexture> _CharatorTex) { m_pCharatorTex = _CharatorTex; }
	void SetBackgroundTex(Ptr<CTexture> _BgTex) { m_pBgTex = _BgTex; }

public:
	CLONE(CDialog);
	CDialog();
	~CDialog();
};
