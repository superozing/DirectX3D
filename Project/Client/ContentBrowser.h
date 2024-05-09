#pragma once
#include "UI.h"
class ContentBrowser :
    public UI
{
private:
    class TreeUI* m_Tree;
    map<string, string> m_vecContentFileName;

private:
    void AddDirectoryNode(class TreeNode* _parent, const wstring& _path);

public:
    void SelectBrowser(DWORD_PTR _Node);
    void ResetBrowser();

public:
    virtual void render_update() override;

public:
    ContentBrowser();
    ~ContentBrowser();
};

