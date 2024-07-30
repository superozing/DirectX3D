#include "pch.h"
#include "CMuzzleFlashScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>

CMuzzleFlashScript::CMuzzleFlashScript()
	: CScript((UINT)SCRIPT_TYPE::MUZZLEFLASHSCRIPT)
	, m_Parent(nullptr)
{
}

CMuzzleFlashScript::~CMuzzleFlashScript()
{
}

void CMuzzleFlashScript::begin()
{
	m_Parent = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Azusa");
}

void CMuzzleFlashScript::tick()
{
	Matrix BoneFrameMat = m_Parent->Animator3D()->FindBoneMat(L"Bip001_Weapon");
	Transform()->SetFrameMat(BoneFrameMat); // x : 19, y : 25, z : -24
}
