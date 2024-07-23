#pragma once
#include "C:\Users\lee\source\repos\PortOrigin\External\Include\Engine\CScript.h"
#include "CWrapImage.h"

class CWallScript : public CScript
{
private:
	WrapMode m_Mode;
	Vec2	 PlaneScale;

	Vec2 OriginTexSize;
	Vec2 WrapSize;
	void ChangeWrapMode();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CWallScript);
	CWallScript();
	~CWallScript();
};
