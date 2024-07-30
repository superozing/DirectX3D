#include "pch.h"
#include "ModelUI.h"

#include "TreeUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CTransform.h>

ModelUI::ModelUI()
	: UI("ModelUI", "##ModelUI")
	, m_bDraw(true)
	, m_bDrawOnlySelected(true)
	, m_fDrawSize(1.f)
{
	m_pBoneTree = new TreeUI("ModelUI");
	m_pBoneTree->UseDragDrop(false);
	m_pBoneTree->ShowRootNode(false);
	AddChildUI(m_pBoneTree);
}

ModelUI::~ModelUI()
{
}

void ModelUI::SetModel(CGameObject* _obj)
{
	// 트리 내용을 삭제
	m_pBoneTree->ClearNode();
	m_pModel = nullptr;

	if (_obj && _obj->Animator3D())
	{
		m_pModel = _obj;

		auto bones = m_pModel->Animator3D()->GetBones();

		vector<TreeNode*> vec(bones->size());

		if (vec.size() <= 0)
			return;

		vec[0] = m_pBoneTree->AddTreeNode(nullptr, ToString(bones->at(0).strBoneName), 0);

		for (int i = 1; i < bones->size(); i++)
		{
			vec[i] = m_pBoneTree->AddTreeNode(vec[bones->at(i).iParentIndx], ToString(bones->at(i).strBoneName), 0);
		}

		m_pBoneTree->AllNodeOpen(true);
	}
}

void ModelUI::tick()
{
	if (!m_pModel || !m_bDraw)
		return;

	const auto& pModelVec = m_pModel->Animator3D()->GetAnimClip();

	if (pModelVec->size() <= 0)
		return;

	pModelVec->at(m_pModel->Animator3D()->GetCurClip()).iFrameLength;

	auto root = m_pBoneTree->GetRootNode();

	if (!root)
		return;

	queue<TreeNode*> q;
	q.push(root);

	Matrix matModel = m_pModel->Transform()->GetWorldMat();

	while (!q.empty())
	{
		auto node = q.front();
		q.pop();

		const Matrix& mat = m_pModel->Animator3D()->FindBoneMat(ToWString(node->GetName()));

		Matrix scaleMat = XMMatrixScalingFromVector(Vec3(m_fDrawSize, m_fDrawSize, m_fDrawSize) * 0.01f);

		if (node->IsSelected())
		{
			GamePlayStatic::DrawDebugCube((scaleMat * mat * matModel), Vec3(1.f, 0.f, 0.f), false);
		}
		else
		{
			if (!m_bDrawOnlySelected)
				GamePlayStatic::DrawDebugCube((scaleMat * mat * matModel), Vec3(0.f, 1.f, 0.f), false);
		}

		const auto& childs = node->GetChildNode();
		for (auto& child : childs)
		{
			q.push(child);
		}
	}
}

void ModelUI::render_update()
{
	ImGui::Checkbox("Draw##ModelUI", &m_bDraw);

	ImGui::Checkbox("DrawOnlySelected##ModelUI", &m_bDrawOnlySelected);

	ImGui::SliderFloat("Size", &m_fDrawSize, 1.f, 100.f);

	ImGui::Separator();
}