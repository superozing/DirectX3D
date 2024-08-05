#pragma once

#include <Engine/CKeyMgr.h>
#include <Engine/singleton.h>

struct PlayerKeyInfo
{
	KEY	  Front;
	KEY	  Right;
	KEY	  Back;
	KEY	  Left;
	KEY	  Dash;
	KEY	  Attack;
	KEY	  Zoom;
	KEY	  Reload;
	KEY	  Cover;
	KEY	  Skill;
	KEY	  Jump;
	KEY	  Flip;
	float Sensitivity;
};

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
	static KEY Flip;

	static float Sensitivity;

	static PlayerKeyInfo GetInfo();
	static void			 SetInfo(PlayerKeyInfo _info);
};
