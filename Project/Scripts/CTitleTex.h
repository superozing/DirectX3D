#pragma once
#include "CImageUIScript.h"
class CTitleTex : public CImageUIScript
{
public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CTitleTex);
	CTitleTex();
	~CTitleTex();
};
