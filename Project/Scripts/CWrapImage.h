#pragma once
#include <Engine\CScript.h>

enum class WrapMode
{
	BasedOnTexture,
	Custom,
};

class CWrapImage : public CScript
{
private:
	WrapMode m_Mode;
	Vec2	 PlaneScale;

	Vec2 OriginTexSize;
	Vec2 WrapSize;

private:
	void ChangeWrapMode();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CWrapImage);
	CWrapImage();
	~CWrapImage();
};
