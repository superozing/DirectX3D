#pragma once
#include "CProjectileScript.h"
class CMegaFistScript : public CProjectileScript
{
private:
	class CGameObject* m_Shooter;  // 투사체를 발사하는 객체
	CGameObject*	   m_Target;   // 투사체가 향하는 객체
	int				   m_CurState; // 슈터의 현재 스테이트

public:
	virtual void OnHit() override;
	void InitMegaFistInfo(CGameObject* _Shooter, CGameObject* _Target, Vec3 _Pos, float _InitSpeed, float _MaxSpeed,
						  float _LifeSpan, float _Damage, bool _Explode, bool _Alive);

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CMegaFistScript);
	CMegaFistScript();
	~CMegaFistScript();
};
