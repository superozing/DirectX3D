#pragma once
#include "UI.h"

// AssetMgr 에 로딩되어 있는 모든 에셋들을 나열

class TreeUI;

class Content :
    public UI
{
private:
    TreeUI*             m_DirectoryTree;
    TreeUI*             m_ContentTree;

    vector<wstring>     m_vecAssetFileName;
    string m_strCurDirectory;

public:
    void ResetBrowser();
    void SelectBrowser(DWORD_PTR _Node);

    void ResetContent();
    void ReloadContent();
    void SelectAsset(DWORD_PTR _Node);
    void SetTargetDirectory(const string & _path);

    ASSET_TYPE GetAssetTypeByExt(const path& _relativePath);

private:
    void AddDirectoryNode(class TreeNode* _parent, const wstring& _path);

public:
    virtual void render_update() override;

public:
    Content();
    ~Content();
};

