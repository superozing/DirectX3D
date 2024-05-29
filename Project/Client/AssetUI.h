#pragma once
#include "UI.h"

#include <Engine/Ptr.h>
#include <Engine/CAsset.h>

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

    void SetAssetKey(Ptr<CAsset> _Asset, const wstring& _Key);

public:
    virtual void render_update() override;

    virtual void CreateAssetInstance(Ptr<CAsset> _Asset);
    virtual void ChangeAssetName(const string& _OriginRelativePath, const string& _NewRelativePath);

public:
    AssetUI(const string& _strName, const string& _ID, ASSET_TYPE _Type);
    ~AssetUI();
};

