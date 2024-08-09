#include "pch.h"
#include "CShootingSystemScript.h"
#include <Engine/CPhysXMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CRandomMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CDamagedDirectionMgr.h"
#include "CBulletMarkSpawner.h"
#include "CBulletShellSpawner.h"
#include "CBulletHitParticleSpawner.h"
#include "CShootingRecoil.h"
#include "CPlayerScript.h"
#include "CDamageFontSpawner.h"
#include "CBulletWarheadSpawner.h"
#include "CBossScript.h"

CShootingSystemScript::CShootingSystemScript()
	: CScript((UINT)SCRIPT_TYPE::SHOOTINGSYSTEMSCRIPT)
	, m_fSpreadRatioSpeed(0.1)
{
}

CShootingSystemScript::~CShootingSystemScript()
{
	if (m_pDamagedDirectionMgr)
		delete m_pDamagedDirectionMgr;
	if (m_pBulletMarkDecalSpawner)
		delete m_pBulletMarkDecalSpawner;
	if (m_pBulletShellSpawner)
		delete m_pBulletShellSpawner;
	if (m_pBulletHitParticleSpawner)
		delete m_pBulletHitParticleSpawner;
	if (m_pShootingRecoil)
		delete m_pShootingRecoil;
	if (m_pDamageFontSpawner)
		delete m_pDamageFontSpawner;
	if (m_pBulletWarheadSpawner)
		delete m_pBulletWarheadSpawner;
}

void CShootingSystemScript::ShootPlayerBulletRay()
{
	// 탄퍼짐
	m_fSpreadRatio;

	tRoRHitInfo hitInfo	  = {};
	auto		pMainCam  = CRenderMgr::GetInst()->GetMainCam();
	auto		FrontDir  = pMainCam->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	float		MaxSpread = 0.3f;

	Vec3 ShootDir = FrontDir + Vec3(m_fSpreadRatio * CRandomMgr::GetInst()->GetRandomFloat() * MaxSpread,
									m_fSpreadRatio * CRandomMgr::GetInst()->GetRandomFloat() * MaxSpread,
									m_fSpreadRatio * CRandomMgr::GetInst()->GetRandomFloat() * MaxSpread);

	bool isBulletHit =
		CPhysXMgr::GetInst()->PerformRaycast(pMainCam->Transform()->GetWorldPos(), ShootDir, hitInfo,
											 (UINT)LAYER::LAYER_PLAYER_SHOOTING_RAY, RayCastDebugFlag::AllInvisible);

	if (isBulletHit)
	{
		// 맞은 오브젝트의 인덱스에 따라서 효과를 주어요.
		switch ((LAYER)hitInfo.pOtherObj->GetLayerIdx())
		{
		case LAYER::LAYER_DEFAULT:
			break;

		case LAYER::LAYER_PLANE:
		case LAYER::LAYER_CEIL:
		case LAYER::LAYER_WALL: {
			m_pBulletMarkDecalSpawner->SpawnBulletMarkDecal(hitInfo, m_pPlayer);
			m_pBulletHitParticleSpawner->SpawnBulletHitParticle(hitInfo);
		}
		break;

		case LAYER::LAYER_PLAYER:
			break;

		case LAYER::LAYER_PLAYER_SKILL:
			break;

		case LAYER::LAYER_MONSTER: {
			if (hitInfo.pOtherObj->PhysX()->m_bPhysBodyType == PhysBodyType::DYNAMIC)
			{
				hitInfo.pOtherObj->PhysX()->applyBulletImpact(
					PxVec3(ShootDir.x, ShootDir.y, ShootDir.z), 3000.f,
					PxVec3(hitInfo.vHitPos.x, hitInfo.vHitPos.y, hitInfo.vHitPos.z));
			}
			m_pDamageFontSpawner->SpawnDamageFont(hitInfo.vHitPos, 10);
			m_pBulletHitParticleSpawner->SpawnBulletHitParticle(hitInfo);
		}
		break;

		case LAYER::LAYER_MONSTER_SKILL:
			break;

		case LAYER::LAYER_BOSS:
			if (hitInfo.pOtherObj->PhysX()->m_bPhysBodyType == PhysBodyType::STATIC)
			{
				hitInfo.pOtherObj->PhysX()->applyBulletImpact(
					PxVec3(ShootDir.x, ShootDir.y, ShootDir.z), 3000.f,
					PxVec3(hitInfo.vHitPos.x, hitInfo.vHitPos.y, hitInfo.vHitPos.z));

				float Dmg = 0.f;

				if (hitInfo.pOtherObj->GetScript<CBossScript>()->IsShield())
					Dmg = 1.f;
				else
					Dmg = m_pPlayerScript->GetDamage();

				hitInfo.pOtherObj->GetScript<CBossScript>()->Hit(Dmg);
			}

			if (hitInfo.pOtherObj->GetScript<CBossScript>()->IsShield())
				m_pDamageFontSpawner->SpawnDamageFont(hitInfo.vHitPos, 1);
			else
				m_pDamageFontSpawner->SpawnDamageFont(hitInfo.vHitPos, (int)m_pPlayerScript->GetDamage());
			m_pBulletHitParticleSpawner->SpawnBulletHitParticle(hitInfo);
			break;

		case LAYER::LAYER_BOSS_SKILL:
			break;

		case LAYER::LAYER_LIGHT:
			break;

		case LAYER::LAYER_EVENT:
			break;

		case LAYER::LAYER_RAYCAST:
			break;

		case LAYER::LAYER_PLAYER_CAMERA_RAY:
			break;

		case LAYER::LAYER_PLAYER_SHOOTING_RAY:
			break;

		case LAYER::LAYER_ETC_OBJECT:
			break;

		case LAYER::LAYER_UI:
			break;

		default:
			break;
		}
	}

	// 무기 본 위치에 탄피 오브젝트 생성, Velocity 추가
	m_pBulletShellSpawner->SpawnBulletShell(m_pPlayer);

	// 무기 총구 위치에 탄두 오브젝트
	m_pBulletWarheadSpawner->SpawnBulletWarhead(m_pPlayer);

	// 반동 적용
	m_pShootingRecoil->ApplyShootingRecoil();

	// SpreadRatio 늘리기
	m_fSpreadRatio += m_fSpreadRatioSpeed * DT;
}

void CShootingSystemScript::ShootMainCamRay()
{
	// 메인 카메라 Raycast
	// int mask = RayCastDebugFlag::EndPointVisible;

	tRoRHitInfo hitInfo	 = {};
	auto		pMainCam = CRenderMgr::GetInst()->GetMainCam();
	auto		FrontDir = pMainCam->Transform()->GetWorldDir(DIR_TYPE::FRONT);

	bool isContact = CPhysXMgr::GetInst()->PerformRaycast(pMainCam->Transform()->GetWorldPos(), FrontDir, hitInfo,
														  (UINT)LAYER::LAYER_RAYCAST, RayCastDebugFlag::AllInvisible);

	m_bShootAvailable = true;

	if (isContact)
		// 메인 카메라 ray에 충돌한 오브젝트가 있을 경우
		m_MainCamAimLayer = (LAYER)hitInfo.pOtherObj->GetLayerIdx();
	else
		// 충돌한 오브젝트가 없을 경우 default로 초기화
		m_MainCamAimLayer = LAYER::LAYER_DEFAULT;
}

void CShootingSystemScript::begin()
{
	// 레벨에서 플레이어 스크립트 가져오기
	m_pPlayer		= CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(PlayerName);
	m_pPlayerScript = m_pPlayer->GetScript<CPlayerScript>();

	m_pBulletMarkDecalSpawner	= new CBulletMarkSpawner;
	m_pBulletShellSpawner		= new CBulletShellSpawner;
	m_pBulletHitParticleSpawner = new CBulletHitParticleSpawner;
	m_pDamageFontSpawner		= new CDamageFontSpawner;
	m_pShootingRecoil			= new CShootingRecoil;
	m_pBulletWarheadSpawner		= new CBulletWarheadSpawner;

	m_pBulletMarkDecalSpawner->begin();
	m_pBulletShellSpawner->begin();
	m_pBulletHitParticleSpawner->begin();
	m_pDamageFontSpawner->begin();
	m_pBulletWarheadSpawner->begin();

	// 윈도우 좌표 기준이기 떄문에 반동을 주기 위해 y를 -방향으로 세팅
	m_pShootingRecoil->SetShootingRecoilValue(Vec2(0.f, -0.f)); // 나중에 수치를 조정할 필요가 있음.
	// 예를 들어 자세에 따라서 다른 반동을 준다던가... 그런 것 들 말이죠.
}

void CShootingSystemScript::tick()
{
	ShootMainCamRay();

	m_pBulletMarkDecalSpawner->tick();
	m_pBulletShellSpawner->tick();
	m_pBulletHitParticleSpawner->tick();
	m_pDamageFontSpawner->tick();
	m_pBulletWarheadSpawner->tick();

	m_fSpreadRatio = RoRMath::ClampFloat(m_fSpreadRatio - DT, 0.1f);
}
