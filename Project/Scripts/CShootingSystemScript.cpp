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
#include "CPlayerScript.h"

CShootingSystemScript::CShootingSystemScript()
	: CScript((UINT)SCRIPT_TYPE::SHOOTINGSYSTEMSCRIPT)
	, m_fSpreadRatioSpeed(0.1)
{
}

CShootingSystemScript::~CShootingSystemScript()
{
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
		CPhysXMgr::GetInst()->PerfomRaycast(pMainCam->Transform()->GetWorldPos(), ShootDir, hitInfo,
											(UINT)LAYER::LAYER_PLAYER_SHOOTING_RAY, RayCastDebugFlag::AllVisible);

	if (isBulletHit)
	{
		// 데미지 처리, 파티클 시스템 등등...
		if (hitInfo.pOtherObj->GetLayerIdx() == (UINT)LAYER::LAYER_PLAYER)
		{
			int a = 0;
		}

		if (hitInfo.pOtherObj->GetLayerIdx() == (UINT)LAYER::LAYER_WALL)
		{
			// 데칼 오브젝트 스폰
			m_pBulletMarkDecalSpawner->SpawnBulletMarkDecal(
				hitInfo.pOtherObj->Transform()->GetWorldDir(DIR_TYPE::FRONT), hitInfo.vHitPos);
		}

		if (hitInfo.pOtherObj->GetLayerIdx() == (UINT)LAYER::LAYER_MONSTER &&
			(hitInfo.pOtherObj->PhysX()->m_bPhysBodyType == PhysBodyType::DYNAMIC))
		{
			hitInfo.pOtherObj->PhysX()->applyBulletImpact(
				PxVec3(ShootDir.x, ShootDir.y, ShootDir.z), 1000.f,
				PxVec3(hitInfo.vHitPos.x, hitInfo.vHitPos.y, hitInfo.vHitPos.z));
		}
	}

	// 무기 본 매트릭스에 탄피 세팅
	m_pBulletShellSpawner->SpawnBulletShell(m_pPlayer);

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

	bool isContact = CPhysXMgr::GetInst()->PerfomRaycast(pMainCam->Transform()->GetWorldPos(), FrontDir, hitInfo,
														 (UINT)LAYER::LAYER_RAYCAST, RayCastDebugFlag::AllVisible);

	m_bShootAvailable = true;

	if (isContact)
		// 메인 카메라 ray에 충돌한 오브젝트가 있을 경우
		m_MainCamAimLayer = (LAYER)hitInfo.pOtherObj->GetLayerIdx();
	else
		// 충돌한 오브젝트가 없을 경우 default로 초기화
		m_MainCamAimLayer = LAYER::LAYER_DEFAULT;
}

void CShootingSystemScript::SpawnBulletShell()
{
}

void CShootingSystemScript::begin()
{
	// 레벨에서 플레이어 스크립트 가져오기
	m_pPlayer		= CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(PlayerName);
	m_pPlayerScript = m_pPlayer->GetScript<CPlayerScript>();

	// m_pDamagedDirectionMgr = new CDamagedDirectionMgr;
	// GetOwner()->AddComponent(m_pDamagedDirectionMgr);

	m_pBulletMarkDecalSpawner = new CBulletMarkSpawner;
	GetOwner()->AddComponent(m_pBulletMarkDecalSpawner);

	m_pBulletShellSpawner = new CBulletShellSpawner;
	GetOwner()->AddComponent(m_pBulletShellSpawner);
}

void CShootingSystemScript::tick()
{
	ShootMainCamRay();

	m_fSpreadRatio = RoRMath::ClampFloat(m_fSpreadRatio - DT, 0.1f);
}

void CShootingSystemScript::SaveToFile(FILE* _File)
{
}

void CShootingSystemScript::SaveToFile(ofstream& fout)
{
}

void CShootingSystemScript::LoadFromFile(FILE* _File)
{
}

void CShootingSystemScript::LoadFromFile(ifstream& fin)
{
}
