#pragma once
#include <Engine\CScript.h>

class CBladeAfterImage : public CScript
{
private:
	class CGameObject* m_Kaiten;
	class CAfterImage* m_AfterImage;

	bool bSet;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CBladeAfterImage);
	CBladeAfterImage();
	~CBladeAfterImage();
};
