#pragma once
#include "UI.h"

#include <Engine/CGameObject.h>

typedef std::function<void(CAsset*, ASSET_TYPE)> ComponentPayloadEvent;

class ComponentUI :
    public UI
{
private:
    CGameObject*    m_TargetObject;
    COMPONENT_TYPE  m_Type;
    string          m_ComponentTitle;

protected:

   ComponentUI*                           m_DelegateUI;
    vector<ComponentPayloadEvent>         m_vecPayloadEvent;

public:
    virtual void render_update() override;
    virtual void ResetUIinfo();

public:
    CGameObject* GetTargetObject() { return m_TargetObject; }
    void SetTargetObject(CGameObject* _Target);
    void SetComponentTitle(const string& _title) { m_ComponentTitle = _title; }
    COMPONENT_TYPE GetType() { return m_Type; }
    
    void CheckPayLoadData(ASSET_TYPE _Type , int iFuncArrNum);
    void SetDelegateFunc(ComponentUI* _pUI) { m_DelegateUI = _pUI; }
    void AddPayLoadEvent(const ComponentPayloadEvent& _Func) { m_vecPayloadEvent.push_back(_Func); }

public:
    ComponentUI(const string& _strName, const string& _ID, COMPONENT_TYPE _Type);
    ~ComponentUI();
};



