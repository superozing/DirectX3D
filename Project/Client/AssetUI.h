#pragma once
#include "UI.h"

#include <Engine/Ptr.h>
#include <Engine/CAsset.h>

typedef std::function<void(CAsset*)> AssetPayloadEvent;

class AssetUI :
    public UI
{
private:
    Ptr<CAsset>     m_Asset;
    ASSET_TYPE      m_Type;

public:
    Ptr<CAsset> GetAsset() { return m_Asset; }
    void SetAsset(Ptr<CAsset> _Asset);

    ASSET_TYPE GetType() { return m_Type; }

protected:

    AssetUI* m_DelegateUI;
    vector<AssetPayloadEvent>   m_vecPayloadEvent;

public:
    virtual void render_update() override;

public:
    void CheckPayLoadData(int iFuncArrNum);
    void SetDelegateFunc(AssetUI* _pUI) { m_DelegateUI = _pUI; }
    void AddPayLoadEvent(const AssetPayloadEvent& _Func) { m_vecPayloadEvent.push_back(_Func);}

public:
    AssetUI(const string& _strName, const string& _ID, ASSET_TYPE _Type);
    ~AssetUI();
};

