#include "pch.h"
#include "CBulletShellSpawner.h"

#include <Engine/CMemoryPoolMgr.h>

#include "CMemoryPoolMgrScript.h"

#define Bullet_ShellPath "prefab/ShootingSystem/Bullet_Shell.pref"

CBulletShellSpawner::CBulletShellSpawner()
	: CScript((UINT)SCRIPT_TYPE::BULLETSHELLSPAWNER)
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
			m_PoolMgr->PushObject(it->first);
			
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

	//Matrix _WeaponBoneMat = _pObj->Transform()->
	//m_pPlayer->Animator3D()->FindBoneMat(L"Bip001_Weapon"), 


	Vec3 _RightDir = _pPlayer->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 _UpDir = _pPlayer->Transform()->GetWorldDir(DIR_TYPE::UP);
	
	// 필요한 정보
	//	1. _WeaponBoneMat
	//		총 위치의 본 매트릭스에 탄피를 생성시키기 위해서.


	// + 90도 회전이 추가되어야 탄피가 앞쪽을 바라본다.

	// 풀에서 오브젝트 가져오기
	auto pBulletShell = m_PoolMgr->PopObject(Bullet_ShellPath);

	// 기본 크기 설정해주기
	pBulletShell->Transform()->SetWorldMat(_ParentWorldMat);
	Vec3 vPos = pBulletShell->Transform()->GetRelativePos();

	vPos += (_RightDir * 50.f);
	vPos += (_UpDir * 50.f);

	pBulletShell->Transform()->SetRelativePos(vPos);

	// 게임 오브젝트 스폰
	GamePlayStatic::SpawnGameObject(pBulletShell, 0);

	// 관리를 위해 리스트에 추가
	m_BulletShellList.push_back({pBulletShell, _ActiveTime});
}

void CBulletShellSpawner::SaveToFile(FILE* _File)
{
}

void CBulletShellSpawner::SaveToFile(ofstream& fout)
{
}

void CBulletShellSpawner::LoadFromFile(FILE* _File)
{
}

void CBulletShellSpawner::LoadFromFile(ifstream& fin)
{
}
