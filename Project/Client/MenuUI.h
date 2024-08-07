#pragma once
#include "UI.h"

class MenuUI : public UI
{
public:
	virtual void render() override;
	virtual void render_update() override;

private:
	void File();
	void Level();
	void GameObject();
	void Asset();
	void View();

public:
	void ScreenControl();

private:
	void GameStart();
	void GameStop();

private:
	void ContentSaveAll();
	void ScreenResize(Vec2 _Resolution, bool IsWindowMode);

public:
	MenuUI();
	~MenuUI();
};
