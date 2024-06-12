#pragma once
#include "UI.h"

// GameObject 정보
//  - 컴포넌트
//  - 스크립트

// Asset 정보
//  - 각 에셋의 정보

#include <Engine/CGameObject.h>
#include <Engine/CAsset.h>

class ComponentUI;
class ScriptUI;
class AssetUI;

class Inspector :
    public UI
{
private:
    CGameObject*        m_TargetObject;
    Ptr<CAsset>         m_TargetAsset;
    bool                m_bPrefab;

    ComponentUI*        m_arrComUI[(UINT)COMPONENT_TYPE::END];
    vector<ScriptUI*>   m_vecScriptUI;

    AssetUI*            m_arrAssetUI[(UINT)ASSET_TYPE::END];


public:
    virtual void tick() override;
    virtual void render_update() override;
    virtual void enter() override;

public:
    void SetTargetObject(CGameObject* _Object, bool _bPrefab = false);
    void SetTargetAsset(Ptr<CAsset> _Asset);

    CGameObject* GetTargetObject() { return m_TargetObject; }

    void RefreshScriptUI();

    void ObjectName();
    void ObjectLayer();
    int PrefabLayer();
    void ObjectComponent();
    void ObjectScript();

    void CheckTargetComponent(COMPONENT_TYPE _type);
    void DeleteTargetComponent(COMPONENT_TYPE _type);
    void DeleteTargetScript(ScriptUI* _Script);
    void MakePrefab();
    void SavePrefab(const string& _Directory, const string& _FileName);

    ComponentUI* GetComponentUI(COMPONENT_TYPE ComType);

    void ResetComponent();

private:
    void CreateChildUI();
    void CreateComponentUI();
    void CreateAssetUI();
    void ResizeScriptUI(UINT _Size);

private:
    void ResetTargetObject();
    void ResetTargetAsset();

public:
    Inspector();
    ~Inspector();

    friend class CImGuiMgr;
};

