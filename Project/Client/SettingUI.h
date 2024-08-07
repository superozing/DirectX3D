#pragma once

#include "UI.h"

class SettingUI : public UI
{
	void (SettingUI::*select_render)();

public:
	virtual void tick() override;
	virtual void render_update() override;
	virtual void Activate() override;

private:
	void Display();
	void Sound();
	void Graphic();
	void Control();

public:
	SettingUI();
	~SettingUI();
};
