#include "pch.h"
#include "CMuzzleFlash_DroidAR.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

CMuzzleFlash_DroidAR::CMuzzleFlash_DroidAR()
	: CScript((UINT)SCRIPT_TYPE::MUZZLEFLASH_DROIDAR)
	, m_pDroid(nullptr)
	, m_Module{}
{
}

CMuzzleFlash_DroidAR::~CMuzzleFlash_DroidAR()
{
}

void CMuzzleFlash_DroidAR::begin()
{
	m_Module = ParticleSystem()->GetParticleModule();
	ParticleSystem()->SetModule(m_Module);

	//Transform()->SetRelativePos(Vec3(-11.f, 16.f, 43.f));
}

void CMuzzleFlash_DroidAR::tick()
{
	Matrix BoneFrameMat = m_pDroid->Animator3D()->FindBoneMat(L"Bip001_Weapon");
	Transform()->SetFrameMat(BoneFrameMat); 
	Transform()->SetRelativePos(Vec3(-11.f, 16.f, 43.f)); // -11.f, 16.f, 43.f
}
