#include "pch.h"
#include "CBulletMarkSpawner.h"

#include <Engine/CMemoryPoolMgr.h>

#include "CMemoryPoolMgrScript.h"
#include <Engine/CPhysXMgr.h>
#include <Engine/CRenderMgr.h>

#define BulletMarkPath "prefab/ShootingSystem/BulletMarkDecal.pref"

CBulletMarkSpawner::CBulletMarkSpawner()
	: CScript((UINT)SCRIPT_TYPE::BULLETMARKSPAWNER)
	, m_iMaxDecalCount(20)
{
}

CBulletMarkSpawner::~CBulletMarkSpawner()
{
}

void CBulletMarkSpawner::begin()
{
	auto pObj = CMemoryPoolMgr::GetInst()->GetEX();
	m_PoolMgr = pObj->GetScript<CMemoryPoolMgrScript>();

	m_NormalTargetTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex");
}

void CBulletMarkSpawner::tick()
{
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
	bool		 spawnFlag = false;
	CGameObject* pDecalObj = nullptr;

	// 만약 최대 개수보다 많은 오브젝트가 생성될 경우
	if (m_BulletDecalList.size() > m_iMaxDecalCount)
	{
		// 기존 front에 있는 데칼 오브젝트를 사용
		pDecalObj = m_BulletDecalList.front().first;
		m_BulletDecalList.pop_front();
	}
	else
	{
		// 풀에서 오브젝트 가져오기
		pDecalObj = m_PoolMgr->PopObject(BulletMarkPath);
		spawnFlag = true;
	}

	

	// 총이 맞은 위치로 총알 자국 데칼 스폰
	pDecalObj->Transform()->SetRelativePos(_HitInfo.vHitPos);

	// NormalTargetTex 중앙에서 normal 값 가져오기
	//Vec3 vNormal = GetCenterNormal();
	
	// 노말 벡터를 z축으로 회전
	//Vec3 vRotNormal = Vec3(-vNormal.y, vNormal.x, vNormal.z);
	//pDecalObj->Transform()->SetRelativeRotation(GetCenterNormal().Normalize());
	
	Vec3 normal = GetCenterNormal().Normalize();

	// 노말 벡터를 XMVECTOR로 변환
	XMVECTOR normalVec = XMVectorSet(normal.x, normal.y, normal.z, 0.0f);

	// 노말 벡터를 월드 공간으로 변환
	normalVec = XMVector3TransformNormal(normalVec, g_Transform.matViewInv);

	// 변환된 벡터를 Vec3로 다시 변환
	Vec3 worldNormal;
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&worldNormal), normalVec);
	
	// 월드 노말 기준으로 값 세팅
	pDecalObj->Transform()->SetRelativeRotation(worldNormal);

	pDecalObj->Decal()->GetDynamicMaterial(0);
	pDecalObj->Decal()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture/particle/AlphaCircle.png"));
	// 오브젝트를 레벨에 스폰
	if (spawnFlag)
		GamePlayStatic::SpawnGameObject(pDecalObj, 0);

	// 리스트에 추가
	m_BulletDecalList.push_back({pDecalObj, _ActiveTime});
}

Vec3 CBulletMarkSpawner::GetCenterNormal()
{
	Vec2 vTexResol  = Vec2(m_NormalTargetTex->GetWidth(), m_NormalTargetTex->GetHeight());

	// 픽셀 배열 가져오기
	tPixel* pixels = m_NormalTargetTex->GetPixels();
	if (!pixels)
	{
		int i = 0;
	}

	// 화면 중앙 좌표
	int centerX = vTexResol.x / 2;
	int centerY = vTexResol.y / 2;

	// 1차원 배열에서 인덱스 계산
	int idx = centerY * vTexResol.x + centerX;

	// 중앙 픽셀 값을 Vec3로 반환
	tPixel& pixel  = pixels[idx];
	//Vec3 normal = {static_cast<float>(pixel.r), static_cast<float>(pixel.g), static_cast<float>(pixel.b)};
	//// 중앙 픽셀 값을 Vec3로 반환 (노말 데이터가 tPixel에 저장된 방식에 따라 변환 필요)
	Vec3 normal = {static_cast<float>(pixel.r) / 255.0f * 2.0f - 1.0f,
					  static_cast<float>(pixel.g) / 255.0f * 2.0f - 1.0f,
					  static_cast<float>(pixel.b) / 255.0f * 2.0f - 1.0f};

	return normal;
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
