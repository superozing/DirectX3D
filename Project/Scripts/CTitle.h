#pragma once

#include <Engine/CScript.h>
class CTitle : public CScript
{
private:
	class CTitleTex* m_pRemnants;
	class CTitleTex* m_pFMod;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CTitle);
	CTitle();
	~CTitle();
};
