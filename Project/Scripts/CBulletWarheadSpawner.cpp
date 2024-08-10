#include "pch.h"
#include "CBulletWarheadSpawner.h"

#include <Engine/CMemoryPoolMgr.h>
#include <Engine/CRandomMgr.h>
#include <Engine/CPhysXMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CRenderMgr.h>

#include "CMemoryPoolMgrScript.h"
#include "CBulletScript.h"

#define Bullet_WarheadPath "prefab/ShootingSystem/BulletLine.pref"

CBulletWarheadSpawner::CBulletWarheadSpawner()
{
}

CBulletWarheadSpawner::~CBulletWarheadSpawner()
{
}

void CBulletWarheadSpawner::begin()
{
	// 메모리 풀 관리자 가져오기
	auto pObj = CMemoryPoolMgr::GetInst()->GetEX();
	m_PoolMgr = pObj->GetScript<CMemoryPoolMgrScript>();
	m_PoolMgr->PoolSet(Bullet_WarheadPath, 10);

	m_pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Azusa");

	m_RelativePos = Vec3(19.f, 24.f, -25.f);
}

void CBulletWarheadSpawner::tick()
{
	for (list<tBulletWarheadNode>::iterator it = m_BulletWarheadList.begin(); it != m_BulletWarheadList.end();)
	{
		// 모든 List를 순회하면서 AccTime 누적
		it->ActiveTime -= DT;

		it->BulletWarhead->Transform()->SetRelativePos(
			it->BulletWarhead->Transform()->GetRelativePos() + it->Dir * 10000 * DT);

		if (it->ActiveTime < 0.f)
		{
			// 풀 매니저에게 반환
			m_PoolMgr->PushObject(Bullet_WarheadPath, it->BulletWarhead);

			// 리스트에서 제거
			it = m_BulletWarheadList.erase(it);
		}
		else
			++it;
	}
}

void CBulletWarheadSpawner::SpawnBulletWarhead(CGameObject* _pPlayer)
{
	// 기본적인 목표
	// 총구 위치로부터 hit pos 까지의 벡터 구하기.
	// 매 틱마다 normalize 후 -> speed * DT 해서 Pos 이동 시키기


	// 1. 총구에서 탄두가 출발해야 하니 총구 위치를 알아야 해요.
	// 2. 총구 위치에서 조준한 방향으로 날아가야 하니 정확한 direction을 알아야 해요.
	//	hitpos를 사용할 수 없는 이유: ray가 맞지 않아도 탄두는 날아가야 함
	// 3. 탄두를 정확한 방향으로 회전시킨 상태에서 날아가야 해요.(o)
	//	기본 방향인 위 쪽이 아니라 플레이어 기준 앞 쪽을 바라본 상태로 날아가야 해요.

	// 풀에서 오브젝트 가져오기
	auto pBulletWarhead = m_PoolMgr->PopObject(Bullet_WarheadPath);

	Matrix _ParentWorldMat = _pPlayer->Transform()->GetWorldMat();
	Matrix _WeaponBoneMat  = _pPlayer->Animator3D()->FindBoneMat(L"Bip001_Weapon");

	// 트랜스폼 세팅
	pBulletWarhead->Transform()->SetDir(_ParentWorldMat.Front().Normalize());

	// 플레이어 월드 매트릭스
	Matrix WeaponMat = _WeaponBoneMat * _pPlayer->Transform()->GetWorldMat();

	// 트랜스폼 세팅
	pBulletWarhead->Transform()->SetDir(_ParentWorldMat.Down().Normalize());
	pBulletWarhead->Transform()->SetRelativePos(WeaponMat.Translation());
	pBulletWarhead->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::BOOL_2, true);
	pBulletWarhead->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1.f, 1.f, 1.f, 0.7f));
	GamePlayStatic::SpawnGameObject(pBulletWarhead, 0);

	// 관리를 위해 리스트에 추가
	m_BulletWarheadList.push_back({pBulletWarhead, 
		CRenderMgr::GetInst()->GetMainCam()->Transform()->GetWorldDir(DIR_TYPE::FRONT), 1.f});
}
