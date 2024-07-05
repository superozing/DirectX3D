#pragma once

#include <Engine/CKeyMgr.h>

class CPlayerController
{
public:
	// Move
	KEY Front = KEY::W;
	KEY Right = KEY::D;
	KEY Back  = KEY::S;
	KEY Left  = KEY::A;
	KEY Dash  = KEY::SPACE;

	KEY Attack = KEY::LBTN;
	KEY Zoom   = KEY::RBTN;
	KEY Reload = KEY::R;

	KEY Cover = KEY::LSHIFT;
	KEY Skill = KEY::F;
	// KEY Jump  = KEY::LCTRL;

public:
	CPlayerController();
	~CPlayerController();
};
