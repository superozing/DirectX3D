#pragma once
#include <Engine/CUIScript.h>

class CPanelUIScript :
    public CUIScript
{
private:
    Vec2            m_vLbtnDownPos;

    Ptr<CTexture>   m_PanelTex;
    bool            m_AllowTexSet;

    bool            m_AllowDragAndDrop;


    // 임시
    bool isFirstTick = true;

public:
    virtual void tick() override;

public:
    virtual void LBtnDown() override;

    void AllowDragAndDrop() { m_AllowDragAndDrop = true; }
    void DisallowDragAndDrop() { m_AllowDragAndDrop = false; }
    
    void AllowTexSet() { m_AllowTexSet = true; }
    void DisallowTexSet() { m_AllowTexSet = false; }

    void SetPanelTex(Ptr<CTexture> _PanelTex) { m_PanelTex = _PanelTex; }

    virtual void SaveToFile(FILE* _File) override;
    virtual void SaveToFile(ofstream& fout) override;
    virtual void LoadFromFile(FILE* _File) override;
    virtual void LoadFromFile(ifstream& fin) override;

public:
    CLONE(CPanelUIScript);
    CPanelUIScript();
    CPanelUIScript(const CPanelUIScript& _Other);
    ~CPanelUIScript();
};