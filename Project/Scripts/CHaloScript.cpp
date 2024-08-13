#include "pch.h"
#include "CHaloScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include "CPlayerScript.h"

CHaloScript::CHaloScript()
	: CScript((UINT)SCRIPT_TYPE::HALOSCRIPT)
	, m_Parent(nullptr)
	, m_ParentBones{}
	, m_CurFrame(0)
	, m_PrevClipIdx(0)
	, m_CurClipIdx(0)
	, m_HeadBoneIdx(0)
{
}

CHaloScript::~CHaloScript()
{
}

void CHaloScript::begin()
{
	if (nullptr == GetOwner()->GetParent())
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

	Matrix BoneFrameMat = m_Parent->Animator3D()->FindBoneMat(m_HeadBoneIdx);
	Transform()->SetFrameMat(BoneFrameMat);
}