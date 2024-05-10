#pragma once
#include "UI.h"

// AssetMgr 에 로딩되어 있는 모든 에셋들을 나열

class TreeUI;

class Content :
    public UI
{
private:
    TreeUI* m_DirectoryTree;
    TreeUI* m_EngineTree;
    TreeUI* m_ContentTree;
    

    vector<wstring>     m_vecAssetFileName;
    string m_strCurDirectory;

public:
    void ResetBrowser();
    void SelectBrowser(DWORD_PTR _Node);

    void ResetEngineAsset();
    void SelectEngineAssetBrowser(DWORD_PTR _Node);

    void ResetEngineContent(ASSET_TYPE _type);

    void ResetContent();
    void SelectAsset(DWORD_PTR _Node);
    void SelectEngineAsset(DWORD_PTR _Node);
    void SetTargetDirectory(const string & _path);


private:
    void DirectoryUI();
    void EngineAssetUI();
    void ContentUI();

    void AddDirectoryNode(class TreeNode* _parent, const wstring& _path);
    ASSET_TYPE GetAssetTypeByExt(const path& _relativePath);

    Ptr<CAsset> GetAsset(ASSET_TYPE _type, string _key);

public:
    virtual void render_update() override;

public:
    Content();
    ~Content();
};

