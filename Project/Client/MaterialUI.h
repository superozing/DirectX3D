#pragma once
#include "AssetUI.h"
class MaterialUI :
    public AssetUI
{
private:
    TEX_PARAM       m_SelectTexParam;
    Ptr<CMaterial>  m_TargetMtrl;

public:
    virtual void render_update() override;


public:
    void SelectTexture(DWORD_PTR _dwData);
    void ShaderSelect(DWORD_PTR _ptr);

    virtual void CreateAssetInstance(Ptr<CAsset> _Asset) override;

    void ChangeAssetName(const string& _OriginName, const string& _NewName);

public:
    MaterialUI();
    ~MaterialUI();
};

