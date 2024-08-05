#pragma once
#include "CImageUIScript.h"
class CTitleTex : public CImageUIScript
{
public:
	void SetColor(Vec4 _color);
	void Draw(bool _draw);

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CTitleTex);
	CTitleTex();
	~CTitleTex();
};
