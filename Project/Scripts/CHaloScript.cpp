#include "pch.h"
#include "CHaloScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include "CStudentScript.h"

CHaloScript::CHaloScript()
	: CScript((UINT)SCRIPT_TYPE::HALOSCRIPT)
	, m_Parent(nullptr)
	, m_ParentBones{}
	, m_CurFrame(0)
	, m_PrevClipIdx(0)
	, m_CurClipIdx(0)
{
}

CHaloScript::~CHaloScript()
{
}

void CHaloScript::begin()
{
	if (nullptr == GetOwner()->GetParent() || nullptr == GetOwner()->GetParent()->GetScript<CStudentScript>())
		return;

	m_Parent = GetOwner()->GetParent();

	Ptr<CMesh> mesh = m_Parent->GetRenderComponent()->GetMesh();
	m_ParentBones	= mesh->GetBones();

	m_CurClipIdx = m_Parent->Animator3D()->GetCurClip();
	m_CurFrame	 = m_Parent->Animator3D()->GetCurFrameIdx();

	for (size_t i = 0; i < m_ParentBones->size(); ++i)
	{
		if (m_ParentBones->at(i).strBoneName == L"Bip001 Head")
		{
			m_HeadBoneIdx = i;
			break;
		}
	}

	m_HaloPos = GetOwner()->Transform()->GetRelativePos();

	CheckHeadBonePos();
}

void CHaloScript::tick()
{
	if (nullptr == m_Parent || !(m_Parent->Animator3D()))
		return;

	if (IsAnimChange())
	{
		if (m_CurFrame >= m_Parent->Animator3D()->GetAnimClip()->at(m_CurClipIdx).iFrameLength)
		{
			m_CurFrame = 0;
		}

		m_PrevClipIdx = m_CurClipIdx;
	}

	m_CurFrame = m_Parent->Animator3D()->GetCurFrameIdx();

	// 새 헤드본 위치 계산
	CheckHeadBonePos();
}

bool CHaloScript::IsAnimChange()
{
	m_CurClipIdx = m_Parent->Animator3D()->GetCurClip();
	return m_CurClipIdx != m_PrevClipIdx;
}

void CHaloScript::CheckHeadBonePos()
{
	if (m_CurFrame >= m_Parent->Animator3D()->GetAnimClip()->at(m_CurClipIdx).iFrameLength)
		return;

	// 부모 오브젝트의 머리본 위치 찾기
	Matrix FinalMat;
	auto   KeyFrame = m_ParentBones->at(m_HeadBoneIdx).vecKeyFrame[m_CurClipIdx][m_CurFrame];
	Matrix MatTrans = Matrix::CreateTranslation(KeyFrame.vTranslate);
	Matrix MatScale = Matrix::CreateScale(KeyFrame.vScale);
	Matrix MatRot	= Matrix::CreateFromQuaternion(KeyFrame.qRot);
	Matrix FrameMat = MatScale * MatRot * MatTrans;

	Transform()->SetFrameMat(FrameMat);
}