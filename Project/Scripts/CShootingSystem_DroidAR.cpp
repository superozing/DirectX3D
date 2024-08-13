#include "pch.h"
#include "CShootingSystem_DroidAR.h"
#include <Engine/CPhysXMgr.h>

#include "CPlayerScript.h"
#include "CDroidAR.h"

CShootingSystem_DroidAR::CShootingSystem_DroidAR()
	: CScript((UINT)SCRIPT_TYPE::SHOOTINGSYSTEM_DROIDAR)
{
}

CShootingSystem_DroidAR::~CShootingSystem_DroidAR()
{
}

void CShootingSystem_DroidAR::SetShootingDirection(CPlayerScript* _pPlayer)
{
	m_ShootingDirection = (_pPlayer->Transform()->GetWorldPos() - m_pDroidAR->Transform()->GetWorldPos()).Normalize();
}

void CShootingSystem_DroidAR::ShootDroidARBulletRay()
{
	tRoRHitInfo hitInfo	  = {};

	bool isBulletHit =
		CPhysXMgr::GetInst()->PerformRaycast(
			m_pDroidAR->Transform()->GetWorldPos(), 
			m_ShootingDirection, hitInfo, 
			(UINT)LAYER::LAYER_MONSTER_SKILL, 
			RayCastDebugFlag::AllVisible);

	if (isBulletHit)
	{
		// 맞은 오브젝트의 인덱스에 따라서 효과를 주어요.
		switch ((LAYER)hitInfo.pOtherObj->GetLayerIdx())
		{
		case LAYER::LAYER_PLANE:
		case LAYER::LAYER_CEIL:
		case LAYER::LAYER_WALL: 
		{
			// 피격 사운드 재생
			m_vecSound[(UINT)ShootingSystemDroidARSoundType::HitWall]->Play(1, 1.f, true);
		}
			break;

		case LAYER::LAYER_PLAYER:
		{
			// 플레이어에게 데미지 넣어주기.
			auto pPlayer = hitInfo.pOtherObj->GetScript<CPlayerScript>();
			pPlayer->Hit(m_pDroidARScript->GetDamageVal());
			
			// 피격 사운드 재생
			m_vecSound[(UINT)ShootingSystemDroidARSoundType::HitPlayer]->Play(1, 1.f, true);
		}
			break;

		default:
			break;
		}
	}
}

void CShootingSystem_DroidAR::begin()
{
	m_vecSound.resize((UINT)ShootingSystemDroidARSoundType::End);
	m_vecSound[(UINT)ShootingSystemDroidARSoundType::HitWall] =
		CAssetMgr::GetInst()->Load<CSound>(SNDSFX_Old_Skill_H_Ex_02_Hit);
	m_vecSound[(UINT)ShootingSystemDroidARSoundType::HitPlayer] =
		CAssetMgr::GetInst()->Load<CSound>(SNDSFX_Shot_Impact_Hit_02);

	m_pDroidAR = GetOwner();
	m_pDroidARScript = GetOwner()->GetScript<CDroidAR>();
}

void CShootingSystem_DroidAR::tick()
{
}
