#include "pch.h"
#include "CCubePushScript.h"

void CCubePushScript::begin()
{
}

void CCubePushScript::tick()
{
}

#include "Engine\CPhysX.h"
void CCubePushScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	bool bIsPlayer = false;

	auto vecScripts = _OtherObj->GetScripts();
	// 플레이어인지 확인
	for (auto& e : vecScripts)
	{
		if ((UINT)SCRIPT_TYPE::PLAYERSCRIPT == e->GetScriptType())
		{
			bIsPlayer = true;
		}
	}
	if (false == bIsPlayer)
		return;

	auto ThisColPos	  = PhysX()->GetColliderPos();
	auto PlayerColPos = _OtherObj->PhysX()->GetColliderPos();
}

void CCubePushScript::SaveToFile(ofstream& fout)
{
}

void CCubePushScript::LoadFromFile(ifstream& fin)
{
}

CCubePushScript::CCubePushScript()
	: CScript((UINT)SCRIPT_TYPE::CUBEPUSHSCRIPT)
{
}

CCubePushScript::~CCubePushScript()
{
}
