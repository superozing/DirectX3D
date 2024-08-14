#pragma once

#include <Engine\CScript.h>

enum class ShootingSystemTurretSoundType
{
	HitWall,
	HitPlayer,
	End,
};

class CTurretShootingSystem : public CScript
{

private:
	// 적 오브젝트
	CGameObject*   m_Target;
	class CTurret* m_pTurretObj;

	// 쏠 방향
	Vec3 m_ShootingDirection;

	vector<Ptr<CSound>> m_vecSound;

	class CDamagedDirectionMgr* m_DamagedDirectionMgr;

public:
	// 사격 방향 맞추기
	Vec3 GetShootDir() { return m_ShootingDirection; }

	// 총알 광선 쏘기
	void ShootTurretBulletRay();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CTurretShootingSystem);
	CTurretShootingSystem();
	~CTurretShootingSystem();

	friend class CBulletShellSpawner;
};
