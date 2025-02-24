﻿#pragma once
#include <Engine/CScript.h>

enum class ShootingSystemSoundType
{
	WallNormalHit,
	MonsterNormalHit,
	MonsterCriticalHit,
	End,
};

class CShootingSystemScript : public CScript
{
	// 사격 시스템
	// 1. 사격 시스템은 플레이어 소유 스크립트
	//		Player begin()에서 생성해서 플레이어의 멤버로 가지고 있기.
	// 2. Crosshair에게 사격 시스템 스크립트를 포인터로 넘김
	//		Crosshair는 매 틱 사격 시스템 컴포넌트에게서 탄퍼짐 정보와 색상 정보를 가져와서
	//		render에 사용하기.

	// 사격 시스템이 지원해야 할 기능
	// 1. ray (카메라 front)

	// 2. ray (사격)
	//		- 플레이어 사격 함수
	//		- 적 사격 함수

private:
	int m_iMaxAmmo;
	int m_iCurAmmo;

	float m_fSpreadRatio;
	float m_fSpreadRatioSpeed;

	// 사격 가능 여부
	bool m_bShootAvailable;

	LAYER m_MainCamAimLayer;

	// 플레이어 오브젝트
	CGameObject*		 m_pPlayer;
	class CPlayerScript* m_pPlayerScript;

	class CDamagedDirectionMgr*			m_pDamagedDirectionMgr;
	class CBulletMarkSpawner*			m_pBulletMarkDecalSpawner;
	class CBulletShellSpawner*			m_pBulletShellSpawner;
	class CBulletHitParticleSpawner*	m_pBulletHitParticleSpawner;
	class CShootingRecoil*				m_pShootingRecoil;
	class CDamageFontSpawner*			m_pDamageFontSpawner;

	Vec3						 m_HitPos;
	class CBulletWarheadSpawner*		m_pBulletWarheadSpawner;

	vector<Ptr<CSound>> m_vecSound;

private:
	float m_RightRatio = 3.f;
	float m_UpRatio = 6.f;

private:
	void SetShootAvailable(bool _Available) { m_bShootAvailable = _Available; }


public:
	int GetMaxAmmo() { return m_iMaxAmmo; }
	int GetCurAmmo() { return m_iCurAmmo; }

	void SetSpreadRatioSpeed(float _fSpreadRatioSpeed) { m_fSpreadRatioSpeed = _fSpreadRatioSpeed; }

	bool IsShootAvailable() const { return m_bShootAvailable; }

	LAYER GetMainCamAimLayer() const { return m_MainCamAimLayer; }

	// 총알 광선 쏘기
	void ShootPlayerBulletRay();

	// 메인 카메라 광선 쏘기
	void ShootMainCamRay();

	// 재장전
	void Reload() 
	{ 
		m_iCurAmmo = m_iMaxAmmo; 
		m_bShootAvailable = true;
	}

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CShootingSystemScript);
	CShootingSystemScript();
	~CShootingSystemScript();

	friend class CBulletShellSpawner;
};