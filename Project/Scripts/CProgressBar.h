#pragma once
#include <Engine\CScript.h>

class CImageUIScript;

class CProgressBar :
    public CScript
{
private:
    // 체력 게이지
    CImageUIScript* m_pGauge;

    // 초상화
    CImageUIScript* m_pPortrait;
    Ptr<CTexture>   m_PortraitTex;

    // 폰트 텍스쳐
    CImageUIScript* m_pImgFont;
    Ptr<CTexture>   m_ImgFontTex;

    // 최대 체력
    int    m_MaxHP;
    
    // 바 별 체력
    int    m_LineHP;

    int m_LerpHP;

    // 현재 체력
    int    m_CurHP;


    CImageUIScript* m_pOddLineHPUI;
    CImageUIScript* m_pEvenLineHPUI;
    CImageUIScript* m_pLinearHPUI;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetPortraitTex(Ptr<CTexture> _PortraitTex);
    void SetImgFontTex(Ptr<CTexture> _ImgFontTex);

    void SetMaxHP(int _MaxHP) { m_MaxHP =  RoRMath::ClampInt(_MaxHP, 0); }
    void SetLineHP(int _LineHP);
    void SetCurHP(int _CurHP) { m_CurHP = RoRMath::ClampInt(_CurHP, 0, m_MaxHP); }

    int GetMaxHP() const { return m_MaxHP; }
    int GetLineHP() const { return m_LineHP; }
    int GetCurHP() const { return m_CurHP; }

    void Add100();
    void Sub100(); 

private:
    void MakeChildObjects();

public:
    CLONE(CProgressBar);
    CProgressBar();
    ~CProgressBar();

};

