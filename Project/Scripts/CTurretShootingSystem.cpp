#include "pch.h"
#include "CTurretShootingSystem.h"

#include <Engine/CPhysXMgr.h>

#include "CPlayerScript.h"
#include "CTurret.h"

CTurretShootingSystem::CTurretShootingSystem()
	: CScript((UINT)SCRIPT_TYPE::TURRETSHOOTINGSYSTEM)
{
}

CTurretShootingSystem::~CTurretShootingSystem()
{
}

void CTurretShootingSystem::ShootTurretBulletRay()
{
	tRoRHitInfo hitInfo = {};

	Vec3 vFrontDir	 = m_pTurretObj->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 LayStartPos = m_pTurretObj->Transform()->GetWorldPos();
	LayStartPos.y += 50.f;

	m_ShootingDirection = vFrontDir;

	bool isBulletHit = CPhysXMgr::GetInst()->PerformRaycast(
		LayStartPos, vFrontDir, hitInfo, (UINT)LAYER::LAYER_MONSTER_SKILL, RayCastDebugFlag::AllVisible);

	if (isBulletHit)
	{
		// 맞은 오브젝트의 인덱스에 따라서 효과를 주어요.
		switch ((LAYER)hitInfo.pOtherObj->GetLayerIdx())
		{
		case LAYER::LAYER_PLANE:
		case LAYER::LAYER_CEIL:
		case LAYER::LAYER_WALL: {
			// 피격 사운드 재생
			m_vecSound[(UINT)ShootingSystemTurretSoundType::HitWall]->Play(1, 1.f, true);
		}
		break;

		case LAYER::LAYER_PLAYER: {
			// 플레이어에게 데미지 넣어주기.
			auto pPlayer = hitInfo.pOtherObj->GetScript<CPlayerScript>();
			pPlayer->Hit(m_pTurretObj->GetDamageVal());

			// 피격 사운드 재생
			m_vecSound[(UINT)ShootingSystemTurretSoundType::HitPlayer]->Play(1, 1.f, true);
		}
		break;

		default:
			break;
		}
	}
}

void CTurretShootingSystem::begin()
{
	m_vecSound.resize((UINT)ShootingSystemTurretSoundType::End);
	m_vecSound[(UINT)ShootingSystemTurretSoundType::HitWall] =
		CAssetMgr::GetInst()->Load<CSound>(SNDSFX_Old_Skill_H_Ex_02_Hit);
	m_vecSound[(UINT)ShootingSystemTurretSoundType::HitPlayer] =
		CAssetMgr::GetInst()->Load<CSound>(SNDSFX_Shot_Impact_Hit_02);

	m_pTurretObj = GetOwner()->GetScript<CTurret>();
}

void CTurretShootingSystem::tick()
{
}
