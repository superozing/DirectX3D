#include "pch.h"
#include "Outliner.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CKeyMgr.h>

#include "CImGuiMgr.h"
#include "CEditorObjMgr.h"
#include "CGameObjectEx.h"
#include "Inspector.h"
#include "RTViewPort.h"
#include "TreeUI.h"

#include <Engine/CTaskMgr.h>

Outliner::Outliner()
	: UI("Outliner", "##Outliner")
	, m_Filter()
	, m_CopyTarget(nullptr)
{
	m_Tree = new TreeUI("OutlinerTree");
	m_Tree->ShowRootNode(false);
	m_Tree->UseDragDrop(true);

	AddChildUI(m_Tree);

	// 트리에 클릭 이벤트 등록
	m_Tree->AddSelectDelegate(this, (Delegate_1)&Outliner::SelectObject);
	m_Tree->AddDragDropDelegate(this, (Delegate_2)&Outliner::DragDropObject);

	// 트리 내용을 현재 레벨의 물체들로 구성
	ResetCurrentLevel();
}

Outliner::~Outliner()
{
}

void Outliner::render_update()
{
	m_Filter.Draw("##NodeFilter");

	if (ImGui::Button("Collapse All"))
	{
		CollapseAllNode();
	}

	ImGui::Separator();

	if (CTaskMgr::GetInst()->GetObjectEvent())
	{
		ResetCurrentLevel();
	}

	if (KEY_TAP_EDITOR(KEY::DEL))
	{
		TreeNode* pNode = m_Tree->GetSelectedNode();
		if (nullptr != pNode)
		{
			CGameObject* pSelectObj = (CGameObject*)pNode->GetData();
			GamePlayStatic::DestroyGameObject(pSelectObj);

			Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
			pInspector->SetTargetObject(nullptr);
		}
	}

	CheckCopy();

	IsFilteredNode(m_Tree->GetRootNode());
}

void Outliner::enter()
{
	ResetCurrentLevel();
}

void Outliner::ResetCurrentLevel()
{
	unordered_map<string, bool> stateMap;
	SaveNodeState(m_Tree->GetRootNode(), stateMap);

	// 트리 내용을 삭제
	m_Tree->ClearNode();

	// 현재 레벨을 가져온다.
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pCurLevel)
		return;

	// 트리에 루트 추가
	TreeNode* pRootNode = m_Tree->AddTreeNode(nullptr, "DummyRoot", 0);

	for (UINT i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		CLayer* pLayer = pCurLevel->GetLayer(i);
		const vector<CGameObject*>& vecParent = pLayer->GetParentObjects();

		for (size_t i = 0; i < vecParent.size(); ++i)
		{
			AddObjectToTree(pRootNode, vecParent[i]);
		}
	}

	TreeNode* pEditorRootNode = m_Tree->AddTreeNode(pRootNode, "Editor Object", 0);

	auto vecEditorObj = CEditorObjMgr::GetInst()->GetEditorGameObjects();

	for (size_t i = 0; i < vecEditorObj.size(); ++i)
	{
		AddObjectToTree(pEditorRootNode, vecEditorObj[i]);
	}

	RestoreNodeState(m_Tree->GetRootNode(), stateMap);
}

void Outliner::CollapseAllNode()
{
	CollapseNode(m_Tree->GetRootNode());
}

void Outliner::AddObjectToTree(TreeNode* _Node, CGameObject* _Object)
{
	TreeNode* pNode = m_Tree->AddTreeNode(_Node, string(_Object->GetName().begin(), _Object->GetName().end()), (DWORD_PTR)_Object);

	const vector<CGameObject*>& vecChild = _Object->GetChild();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddObjectToTree(pNode, vecChild[i]);
	}
}

void Outliner::SelectObject(DWORD_PTR _Node)
{
	TreeNode* pNode = (TreeNode*)_Node;
	CGameObject* pObject = (CGameObject*)pNode->GetData();

	if (nullptr == pObject)
		return;

	Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->SetTargetObject(pObject);	
	RTViewPort* pViewport = (RTViewPort*)CImGuiMgr::GetInst()->FindUI("##Viewport");
	pViewport->SetTargetObject(pObject);
}

void Outliner::DragDropObject(DWORD_PTR _Dest, DWORD_PTR _Source)
{
	if (0 == _Source)
		return;

	TreeNode* pDestNode = (TreeNode*)_Dest;
	TreeNode* pSourceNode = (TreeNode*)_Source;

	CGameObject* pDestObj = nullptr;
	if(nullptr != pDestNode)
		pDestObj = (CGameObject*)pDestNode->GetData();

	CGameObject* pSourceObj = (CGameObject*)pSourceNode->GetData();

	// 부모 오브젝트가 자신의 자식오브젝트의 자식으로 들어가려는 경우는 방지
	if (pDestObj != nullptr && pDestObj->IsAncestor(pSourceObj))
		return;

	if (nullptr == pDestNode)
	{
		int LayerIdx = pSourceObj->DisconnectWithParent();

		// 원래 부모가 없었다
		if (LayerIdx == -1)
			return;

		CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		pCurLevel->AddObject(pSourceObj, LayerIdx, false);
	}
	else
	{		
		pDestObj->AddChild(pSourceObj);
	}

	ResetCurrentLevel();
}

void Outliner::SaveNodeState(TreeNode* _Node, unordered_map<string, bool>& _StateMap)
{
	if (nullptr == _Node)
		return;

	_StateMap[_Node->GetName()] = _Node->m_bOpen;

	for (auto& child : _Node->GetChildNode())
	{
		SaveNodeState(child, _StateMap);
	}
}

void Outliner::RestoreNodeState(TreeNode* _Node, const unordered_map<string, bool>& _StateMap)
{
	if (nullptr == _Node)
		return;

	auto it = _StateMap.find(_Node->GetName());
	if (it != _StateMap.end())
	{
		_Node->m_bOpen = it->second;
	}

	for (auto& child : _Node->GetChildNode())
	{
		RestoreNodeState(child, _StateMap);
	}
}

void Outliner::CollapseNode(TreeNode* _Node)
{
	if (_Node == nullptr)
		return;

	_Node->m_bOpen = false;

	for (auto& child : _Node->GetChildNode())
	{
		CollapseNode(child);
	}
}

bool Outliner::IsFilteredNode(TreeNode* _Node)
{
	if (m_Filter.IsActive())
	{
		bool bFilter = m_Filter.PassFilter(_Node->GetName().c_str());

		for (auto& child : _Node->GetChildNode())
		{
			bFilter |= IsFilteredNode(child);
		}

		_Node->SetFilterState(bFilter);

		if (bFilter)
		{
			TreeNode* ParentNode = _Node;
			while (nullptr != ParentNode)
			{
				ParentNode->m_bOpen = true;
				ParentNode = ParentNode->GetParent();
			}
		}

		return bFilter;
	}
	else
	{
		_Node->SetFilterState(true);

		for (auto& child : _Node->GetChildNode())
		{
			IsFilteredNode(child);
		}

		return true;
	}
}

void Outliner::CheckCopy()
{
	if (KEY_PRESSED_EDITOR(KEY::LCTRL))
	{
		if (KEY_TAP_EDITOR(KEY::C))
		{
			TreeNode* pNode = m_Tree->GetSelectedNode();

			if (nullptr != pNode)
			{
				m_CopyTarget = (CGameObject*)pNode->GetData();
			}
		}
		else if (KEY_TAP_EDITOR(KEY::V))
		{
			if (nullptr != m_CopyTarget)
			{
				int LayerIdx = m_CopyTarget->GetLayerIdx();
				CGameObject* pClone = m_CopyTarget->Clone();
				GamePlayStatic::SpawnGameObject(pClone, LayerIdx);

				Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
				pInspector->SetTargetObject(pClone);
				RTViewPort* pViewport = (RTViewPort*)CImGuiMgr::GetInst()->FindUI("##Viewport");
				pViewport->SetTargetObject(pClone);
			}
		}
	}
}
