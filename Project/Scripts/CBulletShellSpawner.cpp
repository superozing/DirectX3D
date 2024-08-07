#include "pch.h"
#include "CBulletShellSpawner.h"

#include <Engine/CMemoryPoolMgr.h>
#include <Engine/CRandomMgr.h>

#include "CMemoryPoolMgrScript.h"
#include "CBulletScript.h"

#define Bullet_ShellPath "prefab/ShootingSystem/Bullet_Shell.pref"

CBulletShellSpawner::CBulletShellSpawner()
{
}

CBulletShellSpawner::~CBulletShellSpawner()
{
}
	
void CBulletShellSpawner::begin()
{
	// 메모리 풀 관리자 가져오기
	auto pObj = CMemoryPoolMgr::GetInst()->GetEX();
	m_PoolMgr = pObj->GetScript<CMemoryPoolMgrScript>();
	m_PoolMgr->PoolSet(Bullet_ShellPath, 10);
}

void CBulletShellSpawner::tick()
{
	for (list<pair<CGameObject*, float>>::iterator it = m_BulletShellList.begin(); it != m_BulletShellList.end();)
	{
		// 모든 List를 순회하면서 AccTime 누적
		it->second -= DT;

		if (it->second < 0.f)
		{
			// 풀 매니저에게 반환
			m_PoolMgr->PushObject(Bullet_ShellPath, it->first);

			// 리스트에서 제거
			it = m_BulletShellList.erase(it);
		}
		else
			++it;
	}
}

void CBulletShellSpawner::SpawnBulletShell(CGameObject* _pPlayer, float _ActiveTime)
{

	Matrix _ParentWorldMat = _pPlayer->Transform()->GetWorldMat();
	Matrix _WeaponBoneMat = _pPlayer->Animator3D()->FindBoneMat(L"Bip001_Weapon");

	// 풀에서 오브젝트 가져오기
	auto pBulletShell = m_PoolMgr->PopObject(Bullet_ShellPath);

	// 플레이어 무기의 매트릭스 구하기
	Matrix WeaponMat = _WeaponBoneMat * _pPlayer->Transform()->GetWorldMat();

	// 탄피 위치와 회전 설정
	pBulletShell->Transform()->SetRelativePos(WeaponMat.Translation());

	// + 90도 회전이 추가되어야 탄피가 앞쪽을 바라본다.
	pBulletShell->Transform()->SetRelativeRotation(WeaponMat.Up());

	// 힘 주기
	auto pBulletScript = pBulletShell->GetScript<CBulletScript>(); // WeaponMat.Front()
	auto rDir		   = _ParentWorldMat.Right().Normalize();	   // WeaponMat.Right().Normalize();
	
	// 속도 조절 수정 이후 조절하기.
	//  * (CRandomMgr::GetInst()->GetRandomFloat() / 2.f + 1);
	pBulletScript->SetLinearVelocity(rDir * 300.f);

	// 게임 오브젝트 스폰
	GamePlayStatic::SpawnGameObject(pBulletShell, (UINT)LAYER::LAYER_ETC_OBJECT);

	// 관리를 위해 리스트에 추가
	m_BulletShellList.push_back({pBulletShell, _ActiveTime});
}
