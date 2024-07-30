#pragma once
#include <Engine/CScript.h>
class CBulletShellSpawner : public CScript
{
	// - 탄피 생성기
	//	메모리 풀 매니저 스크립트 사용과 탄피 오브젝트 관리를 위해 감싸는 클래스
	// 
	// - 필요한 기능
	//	1. 탄피 오브젝트를 메모리 풀에서 꺼내서 관리
	//	2. 탄피 오브젝트를 일정 시간 후 메모리 풀에 반환
	//	3. 탄피 오브젝트를 레벨에 추가할 때 기본 세팅

private:
	list<pair<CGameObject*, float>> m_BulletShellList;

	class CMemoryPoolMgrScript* m_PoolMgr;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	void SpawnBulletShell(CGameObject* _pObj, float _ActiveTime = 2.f);

public:
	CLONE(CBulletShellSpawner);
	CBulletShellSpawner();
	~CBulletShellSpawner();
};
