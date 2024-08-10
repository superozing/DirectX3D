#pragma once
#include <Engine\CScript.h>

class CGroundCrackScript : public CScript
{
private:
	Vec3 m_Pos; // 데칼 스폰 위치

public:
	void SetDecalPos(Vec3 _Pos) { m_Pos = _Pos; }

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CGroundCrackScript);
	CGroundCrackScript();
	~CGroundCrackScript();
};
