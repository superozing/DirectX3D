#pragma once
#include "Engine\CScript.h"
class CSpawnPhysX : public CScript
{
private:
	tBloom			   m_Info		  = {};
	bool			   m_bBloomActive = true;
	CStructuredBuffer* m_Buffer		  = nullptr;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CSpawnPhysX);
	CSpawnPhysX();
	~CSpawnPhysX();
};