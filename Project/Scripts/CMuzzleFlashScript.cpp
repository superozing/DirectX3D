#include "pch.h"
#include "CMuzzleFlashScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

CMuzzleFlashScript::CMuzzleFlashScript()
	: CScript((UINT)SCRIPT_TYPE::MUZZLEFLASHSCRIPT)
	, m_pTarget(nullptr)
	, m_Module{}
{
}

CMuzzleFlashScript::~CMuzzleFlashScript()
{
}

void CMuzzleFlashScript::begin()
{
	m_Module = ParticleSystem()->GetParticleModule();
	ParticleSystem()->SetModule(m_Module);
	m_pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Azusa");

	Transform()->SetRelativePos(Vec3(19.f, 24.f, -25.f));
}

void CMuzzleFlashScript::tick()
{
	Matrix BoneFrameMat = m_pTarget->Animator3D()->FindBoneMat(L"Bip001_Weapon");
	Transform()->SetFrameMat(BoneFrameMat); // x : 19, y : 25, z : -24
}
