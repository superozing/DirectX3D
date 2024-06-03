#pragma once
#include "UI.h"

#include <Engine/CGameObject.h>

typedef void(*SetPayloadEvent)(CAsset*, ASSET_TYPE);

class ComponentUI :
    public UI
{
private:
    CGameObject*    m_TargetObject;
    COMPONENT_TYPE  m_Type;

    string          m_ComponentTitle;

protected:
    vector<SetPayloadEvent> m_vecPayloadEvent;

public:
    virtual void render_update() override;
    virtual void ResetUIinfo();

public:
    CGameObject* GetTargetObject() { return m_TargetObject; }
    void SetTargetObject(CGameObject* _Target);
    void SetComponentTitle(const string& _title) { m_ComponentTitle = _title; }
    COMPONENT_TYPE GetType() { return m_Type; }
    
    void CheckPayLoadData(ASSET_TYPE _Type , int iFuncArrNum);
    void AddPayloadEvent(SetPayloadEvent _Event) { m_vecPayloadEvent.push_back(_Event); }

public:
    ComponentUI(const string& _strName, const string& _ID, COMPONENT_TYPE _Type);
    ~ComponentUI();
};



