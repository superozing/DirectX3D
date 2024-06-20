#pragma once
#include "CScript.h"

class CUIScript :
    public CScript
{
private:
    CUIScript* m_ParentUI;
    vector<CUIScript*>  m_vecChildUI;
    Vec2                m_vFinalPos;

    bool                m_bMouseCheckAble = true;

    bool                m_bMouseOn;
    bool                m_bMouseOn_Prev;

    bool                m_bMouseLBtnDown;

public:
    virtual void tick() override;

    void AddChildUI(CUIScript* _ChildUI)
    {
        m_vecChildUI.push_back(_ChildUI);
        _ChildUI->m_ParentUI = this;
    }

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

protected:
    CUIScript(UINT _ScriptType);

public:
    CLONE(CUIScript);
    CUIScript();
    CUIScript(const CUIScript& _Other);
    ~CUIScript();

    friend class CUIMgr;
};