#include "pch.h"
#include "ContentBrowser.h"

#include "CImGuiMgr.h"

#include "TreeUI.h"
#include "Content.h"

ContentBrowser::ContentBrowser()
	: UI("ContentBrowser", "##ContentBrowser")
{
	m_Tree = new TreeUI("ContentTree");
	m_Tree->ShowRootNode(false);
	AddChildUI(m_Tree);

	// 트리에 Delegate 를 등록한다.
	m_Tree->AddSelectDelegate(this, (Delegate_1)&ContentBrowser::SelectBrowser);

	ResetBrowser();
}

ContentBrowser::~ContentBrowser()
{
}

void ContentBrowser::AddDirectoryNode(TreeNode* _parent, const wstring& _path)
{
	namespace fs = std::filesystem;
	for (const fs::directory_entry& entry : fs::directory_iterator(_path)) {
		if (entry.is_directory()) {
			auto node = m_Tree->AddTreeNode(_parent, entry.path().filename().string(), 0);
			AddDirectoryNode(node, entry.path().native());
		}
	}
}

void ContentBrowser::SelectBrowser(DWORD_PTR _Node)
{
	TreeNode* pNode = (TreeNode*)_Node;
	string name = pNode->GetName();
	while (pNode->GetParent() && pNode->GetParent()->GetName() != "Root") {
		pNode = pNode->GetParent();
		name = pNode->GetName() + "\\" + name;
	}

	// 선택한 에셋을 Inspector 에게 알려준다.
	Content* pContent = (Content*)CImGuiMgr::GetInst()->FindUI("##Content");
	pContent->SetTargetDirectory(name);

}

void ContentBrowser::ResetBrowser()
{
	// Tree Clear
	m_Tree->ClearNode();

	// 루트노드 추가
	TreeNode* RootNode = m_Tree->AddTreeNode(nullptr, "Root", 0);

	wstring contentPath = CPathMgr::GetContentPath();
	AddDirectoryNode(RootNode, contentPath);
}

void ContentBrowser::render_update()
{
}
