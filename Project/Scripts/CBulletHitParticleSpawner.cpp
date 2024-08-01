﻿#include "pch.h"
#include "CBulletHitParticleSpawner.h"

#include <Engine/CPhysXMgr.h>
#include <Engine/CMemoryPoolMgr.h>

#include "CMemoryPoolMgrScript.h"

#define BulletHitParticlePath "prefab/ShootingSystem/BulletHitParticle.pref"

CBulletHitParticleSpawner::CBulletHitParticleSpawner()
	: CScript((UINT)SCRIPT_TYPE::BULLETHITPARTICLESPAWNER)
{
}

CBulletHitParticleSpawner::~CBulletHitParticleSpawner()
{
}

void CBulletHitParticleSpawner::begin()
{
	// 메모리 풀 관리자 가져오기
	auto pObj = CMemoryPoolMgr::GetInst()->GetEX();
	m_PoolMgr = pObj->GetScript<CMemoryPoolMgrScript>();
}

void CBulletHitParticleSpawner::tick()
{
	for (list<pair<CGameObject*, float>>::iterator it = m_BulletShellList.begin(); it != m_BulletShellList.end();)
	{
		// 모든 List를 순회하면서 AccTime 누적
		it->second -= DT;

		if (it->second < 0.f)
		{
			// 풀 매니저에게 반환
			m_PoolMgr->PushObject(BulletHitParticlePath, it->first);

			// 리스트에서 제거
			it = m_BulletShellList.erase(it);
		}
		else
			++it;
	}
}

void CBulletHitParticleSpawner::SpawnBulletHitParticle(const tRoRHitInfo& _HitInfo)
{
	Vec3 HitPos = _HitInfo.vHitPos;

	// 풀에서 BulletHitParticle 오브젝트 가져오기



	// 게임 오브젝트 스폰
	//GamePlayStatic::SpawnGameObject(pBulletShell, (UINT)LAYER::LAYER_ETC_OBJECT);

	// 관리를 위해 리스트에 추가
	//m_BulletShellList.push_back({pBulletShell, _ActiveTime});
}


void CBulletHitParticleSpawner::SaveToFile(FILE* _File)
{
}

void CBulletHitParticleSpawner::SaveToFile(ofstream& fout)
{
}

void CBulletHitParticleSpawner::LoadFromFile(FILE* _File)
{
}

void CBulletHitParticleSpawner::LoadFromFile(ifstream& fin)
{
}
