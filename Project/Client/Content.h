#pragma once
#include "UI.h"

// AssetMgr 에 로딩되어 있는 모든 에셋들을 나열

class TreeUI;

class Content :
    public UI
{
private:
    TreeUI*             m_Tree;

    vector<wstring>     m_vecAssetFileName;

public:
    void ReloadContent();

    void ResetContent();

    void SelectAsset(DWORD_PTR _Node);
    void SetTargetDirectory(const string & _path);

public:
    virtual void render_update() override;


private:
    void FindFileName(const wstring& _Directory);
    ASSET_TYPE GetAssetTypeByExt(const std::experimental::filesystem::path& _relativePath);

public:
    Content();
    ~Content();
};

