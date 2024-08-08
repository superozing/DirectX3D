#include "pch.h"
#include "CChildCollider.h"

#include <Engine\CGameObject.h>
#include <Engine\CPhysX.h>

CChildCollider::CChildCollider()
	: CScript((UINT)SCRIPT_TYPE::CHILDCOLLIDER)
	, m_Parent(nullptr)
{
}

CChildCollider::~CChildCollider()
{
}

void CChildCollider::begin()
{
	m_Parent = GetOwner()->GetParent();
}

void CChildCollider::tick()
{
	if (nullptr == m_Parent)
		return;
}

void CChildCollider::BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
	auto scripts = m_Parent->GetScripts();
	for (size_t i = 0; i < scripts.size(); ++i)
	{
		scripts[i]->BeginOverlap(m_Parent->PhysX(), _OtherObj, _OtherCollider);
	}
}

void CChildCollider::Overlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
	auto scripts = m_Parent->GetScripts();
	for (size_t i = 0; i < scripts.size(); ++i)
	{
		scripts[i]->Overlap(m_Parent->PhysX(), _OtherObj, _OtherCollider);
	}
}

void CChildCollider::EndOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
	auto scripts = m_Parent->GetScripts();
	for (size_t i = 0; i < scripts.size(); ++i)
	{
		scripts[i]->EndOverlap(m_Parent->PhysX(), _OtherObj, _OtherCollider);
	}
}
