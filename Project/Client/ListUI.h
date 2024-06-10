#pragma once
#include "UI.h"


class ListUI :
    public UI
{
private:
    vector<string>      m_vecStr;
    string              m_strDBClicked;

    CALL_BACK_1         m_CallBackFunc;

    UI*                 m_pUI;
    Delegate_1          m_Func;
    Delegate_3          m_Func3;
    UINT                m_Idx;
    

public:
    void AddString(const string& _str) { m_vecStr.push_back(_str); }
    void AddString(const vector<string>& _vecStr)
    {
        m_vecStr.insert(m_vecStr.end(), _vecStr.begin(), _vecStr.end());
    }

    void SetDbClickCallBack(CALL_BACK_1 _CallBack) { m_CallBackFunc = _CallBack; }
    void SetDbClickDelegate(UI* _Inst, Delegate_1 _MemFunc)
    {
        m_pUI = _Inst;
        m_Func = _MemFunc;
        m_Func3 = nullptr;
    }

    void SetDbClickDelegate(UI* _Inst, Delegate_3 _MemFunc, UINT _Idx = 0)
    {
        m_pUI = _Inst;
        m_Func3 = _MemFunc;
        m_Idx = _Idx;
        m_Func = nullptr;
    }

    string GetDoubleClicked() { return m_strDBClicked; }



public:
    virtual void render_update() override;
    virtual void Deactivate() override;



public:
    ListUI();
    ~ListUI();
};

