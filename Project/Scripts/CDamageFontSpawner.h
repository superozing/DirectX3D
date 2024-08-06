#pragma once
#include <Engine/CScript.h>

struct tDamageFontNode
{
	CGameObject*	DamageFont;
	float			ActiveTime;
	float			MaxTime;
};

class CDamageFontSpawner : public CScript
{
private:
	list<tDamageFontNode>	m_DamageFontList;
	int						m_iMaxDamageFontCount;

	Ptr<CTexture> m_NormalTargetTex;

	class CMemoryPoolMgrScript* m_PoolMgr;

public:
	virtual void begin() override;
	virtual void tick() override;


	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	void SpawnDamageFont(const Vec3& _HitPos, int Damage, float _ActiveTime = 3.f);

public:
	CLONE(CDamageFontSpawner);
	CDamageFontSpawner();
	~CDamageFontSpawner();
};
