#pragma once
#include "AssetUI.h"
class VideoUI : public AssetUI
{
public:
	virtual void render_update() override;

public:
	VideoUI();
	~VideoUI();
};
