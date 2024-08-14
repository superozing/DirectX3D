#pragma once
#include <Engine/CScript.h>

enum class ShootingSystemDroidARSoundType
{
	HitWall,
	HitPlayer,
	End,
};

class CShootingSystem_DroidAR : public CScript
{

private:
	// 적 오브젝트
	CGameObject*					m_pDroidAR;
	class CDroidAR*					m_pDroidARScript;

	// 쏠 방향
	Vec3							m_ShootingDirection;

	vector<Ptr<CSound>>				m_vecSound;

	class CDamagedDirectionMgr*		m_DamagedDirectionMgr;

public:

	// 사격 방향 맞추기
	void SetShootingDirection(class CPlayerScript* _pPlayer);

	// 총알 광선 쏘기
	void ShootDroidARBulletRay();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CShootingSystem_DroidAR);
	CShootingSystem_DroidAR();
	~CShootingSystem_DroidAR();

	friend class CBulletShellSpawner;
};