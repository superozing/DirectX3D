#pragma once

#include "UI.h"

class SettingUI : public UI
{
public:
	virtual void tick() override;
	virtual void render_update() override;

private:
	void Display();
	void Sound();
	void Graphic();
	void Control();

public:
	SettingUI();
	~SettingUI();
};
