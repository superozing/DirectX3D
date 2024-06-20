#include "pch.h"
#include "TreeUI.h"

#include <Engine/CKeyMgr.h>

TreeNode::TreeNode()
	: m_bFrame(false)
	, m_bOpen(false)
	, m_bFilter(false)
{
}

TreeNode::~TreeNode()
{
	Delete_Vec(m_vecChildNode);
}


void TreeNode::GenericTreeRender(UINT _flag, const string& _id)
{
	if (m_Owner && "OutlinerTree" == m_Owner->GetID())
	{
		if (!m_bFilter)
			return;
	}

	if (ImGui::TreeNodeEx(_id.c_str(), _flag))
	{
		m_bOpen = true;
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(m_Owner->GetID().c_str(), &m_Data, sizeof(DWORD_PTR));
			ImGui::Text(m_Name.c_str());
			ImGui::EndDragDropSource();

			// Tree 에 자신이 Drag 된 노드임을 알린다.
			m_Owner->SetDragNode(this);
		}

		else if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_Owner->GetID().c_str());
			if (payload)
			{
				m_Owner->SetDropNode(this);
			}
			ImGui::EndDragDropTarget();
		}

		else
		{
			if (KEY_RELEASED_EDITOR(KEY::LBTN) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				m_Owner->SetSelectedNode(this);
			}
		}

		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
		{
			ImGui::SetNextItemOpen(m_vecChildNode[i]->m_bOpen);
			m_vecChildNode[i]->render_update();
		}

		ImGui::TreePop();
	}
	else
	{
		m_bOpen = false;
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(m_Owner->GetID().c_str(), &m_Data, sizeof(DWORD_PTR));
			ImGui::Text(m_Name.c_str());
			ImGui::EndDragDropSource();

			// Tree 에 자신이 Drag 된 노드임을 알린다.
			m_Owner->SetDragNode(this);
		}
		else
		{
			if (KEY_RELEASED_EDITOR(KEY::LBTN) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				m_Owner->SetSelectedNode(this);
			}
		}
	}
}

void TreeNode::ImageListRender(UINT _flag, const string& _id)
{
	string treeId =  _id;

	// 임시 파일
	Ptr<CTexture> thumb = CAssetMgr::GetInst()->Load<CTexture>(TEXIconFile);

	// 애셋이 아닐 경우
	auto ext = path(m_Name).extension().string();
	if (ext == ".fx") {
		thumb = CAssetMgr::GetInst()->Load<CTexture>(TEXIconGraphicsShader);
	}
	else if (ext == ".lv") {
		thumb = CAssetMgr::GetInst()->Load<CTexture>(TEXIconLevel);
	}
	else if (ext == ".anim") {
		thumb = CAssetMgr::GetInst()->Load<CTexture>(TEXIconAnim);
	}
	// 애셋일 경우
	else {
		auto pAsset = dynamic_cast<CAsset*>((CAsset*)m_Data);
		if (pAsset) {
			auto type = pAsset->GetType();
			switch (type)
			{
			case ASSET_TYPE::MESH:
				thumb = CAssetMgr::GetInst()->Load<CTexture>(TEXIconMesh);
				break;
			case ASSET_TYPE::MESHDATA:
				break;
			case ASSET_TYPE::PREFAB:
				thumb = CAssetMgr::GetInst()->Load<CTexture>(TEXIconPrefab);
				break;
			case ASSET_TYPE::TEXTURE:
				thumb = (CTexture*)pAsset;
				break;
			case ASSET_TYPE::MATERIAL:
				thumb = CAssetMgr::GetInst()->Load<CTexture>(TEXIconMaterial);
				break;
			case ASSET_TYPE::SOUND:
				thumb = CAssetMgr::GetInst()->Load<CTexture>(TEXIconSound);
				break;
			case ASSET_TYPE::COMPUTE_SHADER:
				thumb = CAssetMgr::GetInst()->Load<CTexture>(TEXIconComputeShader);
				break;
			case ASSET_TYPE::GRAPHICS_SHADER:
				thumb = CAssetMgr::GetInst()->Load<CTexture>(TEXIconGraphicsShader);
				break;
			case ASSET_TYPE::FSM:
				thumb = CAssetMgr::GetInst()->Load<CTexture>(TEXIconStateMachine);
				break;
			case ASSET_TYPE::END:
				break;
			default:
				break;
			}
		}
	}

	ImGui::Image(thumb->GetSRV().Get(), ImVec2(64, 64));
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		ImGui::SetDragDropPayload(m_Owner->GetID().c_str(), &m_Data, sizeof(DWORD_PTR));
		ImGui::Text(m_Name.c_str());
		ImGui::EndDragDropSource();

		// Tree 에 자신이 Drag 된 노드임을 알린다.
		m_Owner->SetDragNode(this);
	}

	else if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_Owner->GetID().c_str());
		if (payload)
		{
			m_Owner->SetDropNode(this);
		}
		ImGui::EndDragDropTarget();
	}

	else
	{
		if (KEY_RELEASED_EDITOR(KEY::LBTN) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
		{
			m_Owner->SetSelectedNode(this);
		}
	}
	ImGui::Dummy(ImVec2(0, 4));
	ImGui::TextWrapped(_id.c_str());
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		ImGui::SetDragDropPayload(m_Owner->GetID().c_str(), &m_Data, sizeof(DWORD_PTR));
		ImGui::Text(m_Name.c_str());
		ImGui::EndDragDropSource();

		// Tree 에 자신이 Drag 된 노드임을 알린다.
		m_Owner->SetDragNode(this);
	}

	else if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_Owner->GetID().c_str());
		if (payload)
		{
			m_Owner->SetDropNode(this);
		}
		ImGui::EndDragDropTarget();
	}

	else
	{
		if (KEY_RELEASED_EDITOR(KEY::LBTN) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
		{
			m_Owner->SetSelectedNode(this);
		}
	}
}

void TreeNode::render_update()
{
	string strID = m_Name + m_ID;

	UINT Flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (m_bFrame)
		Flag |= ImGuiTreeNodeFlags_Framed;
	if (m_vecChildNode.empty())	
		Flag |= ImGuiTreeNodeFlags_Leaf;		
	if (m_bSelected)
		Flag |= ImGuiTreeNodeFlags_Selected;

	ImGui::SetNextItemOpen(m_bOpen);

	if (m_bFrame && m_vecChildNode.empty())
		strID = "   " + strID;

	if(!m_Owner->m_bImageTree){
		GenericTreeRender(Flag, strID);
	}
	else {
		strID = m_Name;
		ImageListRender(Flag, strID);
	}

}

UINT TreeUI::NodeID = 0;

TreeUI::TreeUI(const string& _ID)
	: UI("", _ID)
	, m_bShowRoot(true)
	, m_bDragDrop(false)
{	
}

TreeUI::~TreeUI()
{
	delete m_Root;
}

void TreeUI::render_update()
{
	if (nullptr == m_Root)
		return;

	if (m_bShowRoot)
	{
		m_Root->render_update();
	}
	else
	{
		float winX = ImGui::GetWindowSize().x-40.f;
		int colCnt = 0;
		for (size_t i = 0; i < m_Root->m_vecChildNode.size(); ++i)
		{
			if (m_bImageTree) {
				string id = "##imagetree" + std::to_string(i);
				ImGui::Dummy(ImVec2(8,0));
				ImGui::SameLine();
				ImGui::BeginChild(id.c_str(), ImVec2(80, 120));
				m_Root->m_vecChildNode[i]->render_update();
				ImGui::EndChild();

				if ((colCnt + 3) >= winX / 80) {
					colCnt = 0;
				}
				else {
					ImGui::SameLine();
					colCnt++;
				}
			}
			else {
				m_Root->m_vecChildNode[i]->render_update();
			}
		}
	}

	// Delegate 호출
	if (m_bSelectEvent)
	{
		if (m_SelectInst && m_SelectFunc)
		{
			(m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_Selected);
		}
	}


	// 드래그 대상을 특정 노드가 아닌 공중드랍 시킨 경우
	if (KEY_RELEASED_EDITOR(KEY::LBTN) && m_DragNode && !m_DropNode)
	{
		if (m_DragDropInst && m_DragDropFunc)
		{
			(m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_DropNode, (DWORD_PTR)m_DragNode);
		}		
		m_DragNode = nullptr;
	}
	else if (m_bDragDropEvent)
	{
		if (m_DragDropInst && m_DragDropFunc)
		{
			(m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_DropNode, (DWORD_PTR)m_DragNode);
		}

		m_DropNode = nullptr;
		m_DragNode = nullptr;
	}

	m_bSelectEvent = false;
	m_bDragDropEvent = false;
}

TreeNode* TreeUI::AddTreeNode(TreeNode* _Parent, string _strName, DWORD_PTR _dwData)
{
	TreeNode* pNewNode = new TreeNode;
	pNewNode->m_Data = _dwData;
	pNewNode->SetName(_strName);

	// 노드마다 겹치지않는 숫자를 ## 뒤에 ID 로 붙인다.
	UINT id = NodeID++;

	char buff[50] = {};
	sprintf_s(buff, "##%d", id);
	pNewNode->SetID(buff);

	if (nullptr == _Parent)
	{
		assert(!m_Root);

		m_Root = pNewNode;
	}
	else
	{
		_Parent->AddChildNode(pNewNode);
	}
	
	pNewNode->m_Owner = this;

	return pNewNode;
}

void TreeUI::SetSelectedNode(TreeNode* _SelectedNode)
{
	if (m_Selected)
	{
		m_Selected->m_bSelected  = false;
	}

	m_Selected = _SelectedNode;

	if (nullptr != m_Selected)
	{
		m_Selected->m_bSelected = true;
	}		

	m_bSelectEvent = true;
}

void TreeUI::SetDragNode(TreeNode* _DragNode)
{
	m_DragNode = _DragNode;
}

void TreeUI::SetDropNode(TreeNode* _DropNode)
{
	m_DropNode = _DropNode;
	m_bDragDropEvent = true;
}
