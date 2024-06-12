#pragma once
#include "UI.h"

// 현재 레벨에 있는 모든 오브젝트들 나열

class TreeUI;
class TreeNode;
class CGameObject;

class Outliner :
    public UI
{
private:
    TreeUI*     m_Tree;
    ImGuiTextFilter m_Filter;
    CGameObject* m_CopyTarget;

public:
    virtual void render_update() override;
    virtual void enter() override;

    TreeUI* GetTree() { return m_Tree; }

public:
    void ResetCurrentLevel();
    void CollapseAllNode();

private:
    void AddObjectToTree(TreeNode* _Node, CGameObject* _Object);
    void SelectObject(DWORD_PTR _Node);
    void DragDropObject(DWORD_PTR _Dest, DWORD_PTR _Source);

    void SaveNodeState(TreeNode* _Node, unordered_map<string, bool>& _StateMap);
    void RestoreNodeState(TreeNode* _Node,const unordered_map<string, bool>& _StateMap);
    void CollapseNode(TreeNode* _Node);

    bool IsFilteredNode(TreeNode* _Node);

    void CheckCopy();

public:
    Outliner();
    ~Outliner();
};

