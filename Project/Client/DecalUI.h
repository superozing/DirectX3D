#pragma once
#include "ComponentUI.h"

class DecalUI : public ComponentUI
{
private:
	string strDecalMtrl;
	string strEmissiveCondition;
	int	   iDecalPriority;
	float  fDecalRenderDistance;
	string strCustomAlpha;
	float  fCustomAlpha;

public:
	virtual void render_update() override;

public:
	DecalUI();
	~DecalUI();
};
