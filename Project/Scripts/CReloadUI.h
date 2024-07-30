#pragma once
#include <Engine\CScript.h>

struct tBulletAtlasData
{
	Vec2 LeftTop;
	Vec2 Size;
};

class CReloadUI : public CScript
{
private:
	vector<class CAtlasImageUIScript*> m_vecBullets;
	tBulletAtlasData				   m_tBlankBullet;
	tBulletAtlasData				   m_tFillBullet;

	float m_fReloadTime;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CReloadUI);
	CReloadUI();
	~CReloadUI();
};
