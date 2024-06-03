#pragma once
#include "UI.h"

#include <Engine/CGameObject.h>

typedef void(*SetPayloadEvent)(DWORD_PTR);

class ComponentUI :
    public UI
{
private:
    CGameObject*    m_TargetObject;
    COMPONENT_TYPE  m_Type;

    string          m_ComponentTitle;

protected:
    SetPayloadEvent m_PayloadEvent;

public:
    virtual void render_update() override;
    virtual void ResetUIinfo();

public:
    CGameObject* GetTargetObject() { return m_TargetObject; }
    void SetTargetObject(CGameObject* _Target);
    void SetComponentTitle(const string& _title) { m_ComponentTitle = _title; }
    COMPONENT_TYPE GetType() { return m_Type; }
    
    template <typename T>
    void CheckPayLoadData(T _type);

public:
    ComponentUI(const string& _strName, const string& _ID, COMPONENT_TYPE _Type);
    ~ComponentUI();
};

template<typename T>
inline void ComponentUI::CheckPayLoadData(T _type)
{
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
        if (payload)
        {
            DWORD_PTR data = *((DWORD_PTR*)payload->Data);
            if (data == 0) return;

            m_PayloadEvent(data);

        }

        ImGui::EndDragDropTarget();
    }
}
