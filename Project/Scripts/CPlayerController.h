#pragma once

#include <Engine/CKeyMgr.h>
#include <Engine/singleton.h>

class CPlayerController : public CSingleton<CPlayerController>
{
	SINGLE(CPlayerController);

public:
	// Move
	static KEY Front;
	static KEY Right;
	static KEY Back;
	static KEY Left;
	static KEY Dash;

	static KEY Attack;
	static KEY Zoom;
	static KEY Reload;

	static KEY Cover;
	static KEY Skill;
	static KEY Jump;
};
