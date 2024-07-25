﻿#pragma once
#include "ComponentUI.h"

class DecalUI : public ComponentUI
{
private:
	string	   strDecalMtrl;
	string	   strEmissiveCondition;
	int		   iDecalPriority;
	float	   fDecalRenderDistance;
	string	   strCustomAlpha;
	float	   fCustomAlpha;
	string	   strDecalAction;
	float	   fDecalActionTime;
	Vec4	   AnimationOutlinerColor;
	Vec4	   AnimationOutsideColor;
	Vec4	   AnimationInsideColor;
	DecalShape UIDecalShape;
	DecalType  UIDecalType;

public:
	virtual void render_update() override;

	void MaterialSelect(DWORD_PTR _ptr);

public:
	DecalUI();
	~DecalUI();
};
