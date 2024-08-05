#include "pch.h"
#include "CBulletScript.h"

CBulletScript::CBulletScript()
	: CScript((UINT)SCRIPT_TYPE::BULLETSCRIPT)
{
}

CBulletScript::~CBulletScript()
{
}

void CBulletScript::begin()
{
	//auto pObj = GetOwner();
	//pObj->PhysX()->setLinearVelocity(m_LinearVelocity);
	m_FirstFlag = false;
}

void CBulletScript::tick()
{
	if (m_FirstFlag == false)
	{
		auto pObj = GetOwner();
		pObj->PhysX()->setLinearVelocity(m_LinearVelocity);
		m_FirstFlag = true;
	}
}
