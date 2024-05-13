#include "pch.h"
#include "Content.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CTaskMgr.h>

#include "CImGuiMgr.h"
#include "Inspector.h"
#include "TreeUI.h"

Content::Content()
	: UI("Content", "##Content")
{
	m_DirectoryTree = new TreeUI("DirectoryTree");
	m_DirectoryTree->ShowRootNode(false);
	// 트리에 Delegate 를 등록한다.
	m_DirectoryTree->AddSelectDelegate(this, (Delegate_1)&Content::SelectBrowser);

	m_EngineTree = new TreeUI("EngineTree");
	m_EngineTree->ShowRootNode(false);
	// 트리에 Delegate 를 등록한다.
	m_EngineTree->AddSelectDelegate(this, (Delegate_1)&Content::SelectEngineAssetBrowser);

	// ContentUI 자식으로 Tree 를 지정
	m_ContentTree = new TreeUI("ContentTree");
	m_ContentTree->ShowRootNode(false);
	// 트리에 Delegate 를 등록한다.
	m_ContentTree->AddSelectDelegate(this, (Delegate_1)&Content::SelectAsset);
	m_ContentTree->SetImageTree(true);


	ResetBrowser();
	ResetEngineAsset();
	// AssetMgr 의 에셋상태를 트리에 적용한다.
	ResetContent();

}

Content::~Content()
{
	if (m_ContentTree) 
	{
		delete m_ContentTree;
		m_ContentTree = nullptr;
	}

	if (m_EngineTree)
	{
		delete m_EngineTree;
		m_EngineTree = nullptr;
	}

	if (m_DirectoryTree)
	{
		delete m_DirectoryTree;
		m_DirectoryTree = nullptr;
	}
}

void Content::AddDirectoryNode(TreeNode* _parent, const wstring& _path)
{
	namespace fs = std::filesystem;
	for (const fs::directory_entry& entry : fs::directory_iterator(_path)) {
		if (entry.is_directory()) {
			auto node = m_DirectoryTree->AddTreeNode(_parent, entry.path().filename().string(), 0);
			AddDirectoryNode(node, entry.path().native());
		}
	}
}

void Content::render_update()
{
	if (CTaskMgr::GetInst()->GetAssetEvent())
	{
			ResetContent();
	}

	static float w = 200.0f;
	float h = ImGui::GetWindowSize().y- 40.f;
	if (h < 1.f) h = 1.f;
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::BeginChild("##Browser", ImVec2(w, h), true);

	DirectoryUI();
	ImGui::Separator();
	EngineAssetUI();

	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::InvisibleButton("vsplitter", ImVec2(8.0f, h));
	if (ImGui::IsItemActive())
		w += ImGui::GetIO().MouseDelta.x;
	ImGui::SameLine();
	ImGui::BeginChild("##Content", ImVec2(0, h), true);
	ContentUI();
	ImGui::EndChild();

	//ImGui::InvisibleButton("hsplitter", ImVec2(-1, 8.0f));
	//if (ImGui::IsItemActive())
	//	h += ImGui::GetIO().MouseDelta.y;
	//ImGui::BeginChild("child3", ImVec2(0, 0), true);
	//ImGui::EndChild();

	ImGui::PopStyleVar();
}

void Content::ResetBrowser()
{
	// Tree Clear
	m_DirectoryTree->ClearNode();

	// 루트노드 추가
	TreeNode* RootNode = m_DirectoryTree->AddTreeNode(nullptr, "Root", 0);

	wstring contentPath = CPathMgr::GetContentPath();
	AddDirectoryNode(RootNode, contentPath);
}

void Content::SelectBrowser(DWORD_PTR _Node)
{
	TreeNode* pNode = (TreeNode*)_Node;
	string name = pNode->GetName();
	while (pNode->GetParent() && pNode->GetParent()->GetName() != "Root") {
		pNode = pNode->GetParent();
		name = pNode->GetName() + "\\" + name;
	}

	SetTargetDirectory(name);

	// 트리에 Delegate 를 등록한다.
	m_ContentTree->AddSelectDelegate(this, (Delegate_1)&Content::SelectAsset);
}

void Content::ResetEngineAsset()
{
	// Tree Clear
	m_EngineTree->ClearNode();

	// 루트노드 추가
	TreeNode* RootNode = m_EngineTree->AddTreeNode(nullptr, "Root", 0);
	TreeNode* root = m_EngineTree->AddTreeNode(RootNode, "Engine Assets", 0);
	for (int i = 0; i < (int)ASSET_TYPE::END; i++) {
		m_EngineTree->AddTreeNode(root, ToString(magic_enum::enum_name((ASSET_TYPE)i)), 0);
	}
}

void Content::SelectEngineAssetBrowser(DWORD_PTR _Node)
{
	TreeNode* pNode = (TreeNode*)_Node;
	string name = pNode->GetName();

	auto type = magic_enum::enum_cast<ASSET_TYPE>(name);
	if (!type.has_value()) return;

	ResetEngineContent(type.value());
	m_strCurDirectory = name;

	// 트리에 Delegate 를 등록한다.
	m_ContentTree->AddSelectDelegate(this, (Delegate_1)&Content::SelectEngineAsset);
}

void Content::ResetEngineContent(ASSET_TYPE _type)
{
	// Tree Clear
	m_ContentTree->ClearNode();

	// 루트노드 추가
	TreeNode* RootNode = m_ContentTree->AddTreeNode(nullptr, "Root", 0);

	auto assetMap = CAssetMgr::GetInst()->GetAssets(_type);
	for (auto iter = assetMap.begin(); iter != assetMap.end(); ++iter) {
		if (!iter->second->IsEngineAsset()) continue;

		m_ContentTree->AddTreeNode(RootNode, ToString(iter->second->GetKey()), (DWORD_PTR)iter->second.Get());
	}
}

void Content::ResetContent()
{
	// Tree Clear
	m_ContentTree->ClearNode();

	// 루트노드 추가
	TreeNode* RootNode = m_ContentTree->AddTreeNode(nullptr, "Root", 0);

	string path = ToString(CPathMgr::GetContentPath()) + m_strCurDirectory;
	namespace fs = std::filesystem;
	for (const fs::directory_entry& entry : fs::directory_iterator(path)) {
		if (!entry.is_directory()) {
			auto node = m_ContentTree->AddTreeNode(RootNode, entry.path().filename().string(), 0);
		}
	}
}

void Content::SelectAsset(DWORD_PTR _Node)
{
	TreeNode* pNode = (TreeNode*)_Node;

	if (nullptr == pNode)
		return;

	auto name = pNode->GetName();
	string key = m_strCurDirectory + "\\" + name;

	// 선택한 에셋을 Inspector 에게 알려준다.
	Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");

	// asset 일 경우
	ASSET_TYPE assetType = CAssetMgr::GetInst()->GetAssetTypeByExt(name);
	if (assetType != ASSET_TYPE::END) {
		Ptr<CAsset> pAsset = CAssetMgr::GetInst()->GetAsset(assetType, key);
		
		pInspector->SetTargetAsset(pAsset);

		return;
	}
}

void Content::SelectEngineAsset(DWORD_PTR _Node)
{
	TreeNode* pNode = (TreeNode*)_Node;

	if (nullptr == pNode)
		return;

	auto name = pNode->GetName();

	// 선택한 에셋을 Inspector 에게 알려준다.
	Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");

	Ptr<CAsset> pAsset = CAssetMgr::GetInst()->GetAsset(magic_enum::enum_cast<ASSET_TYPE>(m_strCurDirectory).value(), name);

	pInspector->SetTargetAsset(pAsset);

	return;
}

void Content::SetTargetDirectory(const string & _path)
{
	m_strCurDirectory = _path;
	ResetContent();
}

void Content::DirectoryUI()
{
	if (ImGui::Button("+Add"))
	{
		ImGui::OpenPopup("MakeNew");
	}

	if (ImGui::BeginPopup("MakeNew"))
	{
		// 팝업 창 내부 UI 구현
		if (ImGui::MenuItem("Create Empty Object", ""))
		{
			CGameObject* pNewObj = new CGameObject;
			pNewObj->SetName(L"New GameObject");
			GamePlayStatic::SpawnGameObject(pNewObj, 0);

			// Inspector 의 타겟정보를 새로 만든걸로 갱신
			Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
			pInspector->SetTargetObject(pNewObj);
		}
		if (ImGui::MenuItem("Create Empty Material"))
		{
			wchar_t szPath[255] = {};
			wstring FilePath = CPathMgr::GetContentPath();

			int num = 0;
			while (true)
			{
				swprintf_s(szPath, L"Material//New Material_%d.mtrl", num);
				if (!exists(FilePath + szPath))
					break;
				++num;
			}

			CMaterial* pMtrl = new CMaterial;
			pMtrl->SetName(szPath);
			pMtrl->Save(szPath);
			GamePlayStatic::AddAsset(pMtrl);
			SetTargetDirectory("material");
		}
		ImGui::EndPopup();
	}
	m_DirectoryTree->render_update();
}

void Content::EngineAssetUI()
{
	m_EngineTree->render_update();
}

void Content::ContentUI()
{
	m_ContentTree->render_update();
}
