#pragma once
#include <Engine\CScript.h>

class CMuzzleFlashScript : public CScript
{
private:
	class CGameObject* m_pTarget;
	tParticleModule	   m_Module;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CMuzzleFlashScript);
	CMuzzleFlashScript();
	~CMuzzleFlashScript();
};
