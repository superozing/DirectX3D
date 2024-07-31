#include "pch.h"
#include "CBulletMarkSpawner.h"

#include <Engine/CMemoryPoolMgr.h>

#include "CMemoryPoolMgrScript.h"
#include <Engine/CPhysXMgr.h>
#include <Engine/CRenderMgr.h>

#define BulletMarkPath "prefab/ShootingSystem/BulletMarkDecal.pref"

CBulletMarkSpawner::CBulletMarkSpawner()
	: CScript((UINT)SCRIPT_TYPE::BULLETMARKSPAWNER)
{
}

CBulletMarkSpawner::~CBulletMarkSpawner()
{
}

static bool debugSpawnCheck = false;

void CBulletMarkSpawner::begin()
{
	AppendScriptParam("Spawn Bullet Marker Decal", SCRIPT_PARAM::BOOL, &debugSpawnCheck);
	auto pObj = CMemoryPoolMgr::GetInst()->GetEX();
	m_PoolMgr = pObj->GetScript<CMemoryPoolMgrScript>();
	// begin에서 임시로 생성
	//m_PoolMgr->PushObject(m_PoolMgr->PopObject(ObjPath));
}

void CBulletMarkSpawner::tick()
{
	// 디버그 용 bool값 체크 후 스폰
	if (debugSpawnCheck)
	{
	}

	for (list<pair<CGameObject*, float>>::iterator it = m_BulletDecalList.begin(); it != m_BulletDecalList.end();)
	{
		// 모든 List를 순회하면서 AccTime 누적
		it->second -= DT;

		if (it->second < 0.f)
		{
			m_PoolMgr->PushObject(it->first);
			it = m_BulletDecalList.erase(it);
		}
		else
			++it;
	}
}

void CBulletMarkSpawner::SpawnBulletMarkDecal(const tRoRHitInfo& _HitInfo, CGameObject* _pPlayer, float _ActiveTime)
{
	// 필요한 정보
	//	1. 플레이어 front dir vector
	//		반전시켜서 데칼 방향 고정하기
	//	2. hit pos
	//		데칼 오브젝트를 스폰할 위치

	// 피격 오브젝트
	CGameObject* pHitObj = _HitInfo.pOtherObj;

	// 풀에서 오브젝트 가져오기
	auto pDecalObj = m_PoolMgr->PopObject(BulletMarkPath);

	// 총이 맞은 위치로 총알 자국 데칼 스폰
	pDecalObj->Transform()->SetRelativePos(_HitInfo.vHitPos);


	// dir 구하기

	// 카메라 위치 (예: 원점)
	Vector3 cameraPosition(0.0f, 0.0f, 0.0f);

	// 오브젝트 위치 (hit position) 정의
	Vector3 hitPosition(5.0f, 0.0f, 5.0f);

	// 오브젝트가 카메라를 바라보도록 설정
	Vector3 objectToCamera = cameraPosition - hitPosition;
	objectToCamera.Normalize();

	// 오브젝트 방향 벡터를 각도로 변환
	float objectAngle = std::atan2(objectToCamera.z, objectToCamera.x);

	// 각도를 가장 가까운 직각 각도로 스냅
	float snappedAngle = (int)std::round(objectAngle / XM_PIDIV2) * XM_PIDIV2;

	// 스냅된 각도로 새로운 오브젝트 방향 계산
	Vec3 snappedDirection = Vec3(std::cos(snappedAngle), 0.0f, std::sin(snappedAngle));

		
	// 플레이어 전방 방향 벡터 반전시켜 설정
	pDecalObj->Transform()->SetDir(snappedDirection);
	
	GamePlayStatic::SpawnGameObject(pDecalObj, 0);

	// 리스트에 추가
	//_ActiveTime
	m_BulletDecalList.push_back({pDecalObj, 90});
}

void CBulletMarkSpawner::SaveToFile(FILE* _File)
{
}

void CBulletMarkSpawner::SaveToFile(ofstream& fout)
{
}

void CBulletMarkSpawner::LoadFromFile(FILE* _File)
{
}

void CBulletMarkSpawner::LoadFromFile(ifstream& fin)
{
}
