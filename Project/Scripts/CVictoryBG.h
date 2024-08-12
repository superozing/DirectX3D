#pragma once
#include "CAtlasImageUIScript.h"

class CVictoryBG : public CAtlasImageUIScript
{
private:
	class CGameObject* m_BossLV;
	int				   m_LVState;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CVictoryBG);
	CVictoryBG();
	~CVictoryBG();
};
