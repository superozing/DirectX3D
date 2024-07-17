#pragma once
#include "ComponentUI.h"
class DecalUI : public ComponentUI
{
public:
	virtual void render_update() override;

public:
	DecalUI();
	~DecalUI();
};
