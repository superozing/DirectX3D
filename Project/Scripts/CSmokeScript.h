#pragma once
#include <Engine\CScript.h>

class CSmokeScript : public CScript
{
private:
	class CGameObject* m_Kaiten; // 카이텐져

	vector<CGameObject*> m_Child; // 하위 연기 파티클

	Vec3					m_vDir; // 연기 방향
	tParticleModule			m_Module;
	vector<tParticleModule> m_vecModule;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CSmokeScript);
	CSmokeScript();
	~CSmokeScript();
};
