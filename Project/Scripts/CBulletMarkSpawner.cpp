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

	CMemoryPoolMgr::GetInst()->PushObject("prefab/BulletMarkDecal.pref", CMemoryPoolMgr::GetInst()->PopObject("prefab/BulletMarkDecal.pref"));
}

void CBulletMarkSpawner::tick()
{
	// 디버그 용 bool값 체크 후 스폰
	if (debugSpawnCheck)
	{

	}

	// 모든 List를 순회하면서 AccTime 누적
	// 활성화 시간이 끝났을 경우 메모리 풀 매니저에게 반환
}

void CBulletMarkSpawner::SpawnBulletMarkDecal()
{
	// 필요한 정보
	//	1. 플레이어 front dir vector
	//		반전시켜서 데칼 방향 고정하기
	//	2. hit pos
	//		데칼 오브젝트를 스폰할 위치


}

void CBulletMarkSpawner::SetParentPanelUI()
{
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
