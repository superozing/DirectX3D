#pragma once
#include <Engine\CScript.h>

class CParticleSpawnScript : public CScript
{
private:
	Vec3 m_Pos; // 파티클 스폰 위치

	float m_AccTime;  // 파티클 재생 시간
	float m_Duration; // 파티클 현재 재생 시간
	bool  m_Repeat;	  // 파티클 반복 여부

public:
	void SetParticleInfo(Vec3 _Pos, float _time, bool _Repeat = false);

	void SetParticlePos(Vec3 _Pos) { m_Pos = _Pos; }
	void SetParticleTime(float _time) { m_AccTime = _time; }

	Vec3  GetParticlePos() { return m_Pos; }
	float GetParticleTime() { return m_AccTime; }

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CParticleSpawnScript);
	CParticleSpawnScript();
	CParticleSpawnScript(const CParticleSpawnScript& _Origin);
	~CParticleSpawnScript();
};
