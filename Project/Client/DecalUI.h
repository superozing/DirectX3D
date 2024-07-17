#pragma once
#include "ComponentUI.h"
class DecalUI : public ComponentUI
{
private:
	string strEmissiveCondition;

public:
	virtual void render_update() override;

public:
	DecalUI();
	~DecalUI();
};
