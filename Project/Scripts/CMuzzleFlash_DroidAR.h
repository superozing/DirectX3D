#pragma once
#include <Engine/CScript.h>
class CMuzzleFlash_DroidAR : public CScript
{
private:
	class CGameObject* m_pDroid;
	tParticleModule	   m_Module;

public:
	void SetDroid(CGameObject* _pDroid) { m_pDroid = _pDroid; }

	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CMuzzleFlash_DroidAR);
	CMuzzleFlash_DroidAR();
	~CMuzzleFlash_DroidAR();
};
