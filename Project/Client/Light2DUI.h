#pragma once
#include "ComponentUI.h"

class Light2DUI : public ComponentUI
{
public:
	Vec3 AngleToVector(float _angle);

public:
	virtual void render_update() override;

public:
	Light2DUI();
	~Light2DUI();
};
