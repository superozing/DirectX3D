#include "pch.h"
#include "CBulletMarkSpawner.h"

#include <Engine/CMemoryPoolMgr.h>

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

	CMemoryPoolMgr::GetInst()->PushObject("prefab/BulletMarkDecal.pref",
										  CMemoryPoolMgr::GetInst()->PopObject("prefab/BulletMarkDecal.pref"));
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
			CMemoryPoolMgr::GetInst()->PushObject("prefab/BulletMarkDecal.pref", it->first);
			it = m_BulletDecalList.erase(it);
		}
		else
			++it;
	}
}

void CBulletMarkSpawner::SpawnBulletMarkDecal(Vec3 _PlayerFrontDir, Vec3 _HitPos, float _ActiveTime)
{
	// 필요한 정보
	//	1. 플레이어 front dir vector
	//		반전시켜서 데칼 방향 고정하기
	//	2. hit pos
	//		데칼 오브젝트를 스폰할 위치

	// 풀에서 오브젝트 가져오기
	auto pDecalObj = CMemoryPoolMgr::GetInst()->PopObject("prefab/BulletMarkDecal.pref");

	// 총이 맞은 위치로 총알 자국 데칼 스폰
	pDecalObj->Transform()->SetRelativePos(_HitPos);

	// 플레이어 전방 방향 벡터 반전시켜 설정
	pDecalObj->Transform()->SetDir(-_PlayerFrontDir);
	pDecalObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 50.f));
	pDecalObj->Decal()->GetDynamicMaterial(0);
	pDecalObj->Decal()->GetMaterial(0)->SetTexParam(
		TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture/fx/FX_TEX_Hit_3a.png"));

	// 리스트에 추가
	m_BulletDecalList.push_back({pDecalObj, _ActiveTime});
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
