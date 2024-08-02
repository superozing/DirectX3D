#include "pch.h"
#include "CPlayerController.h"

KEY CPlayerController::Front = KEY::W;
KEY CPlayerController::Right = KEY::D;
KEY CPlayerController::Back	 = KEY::S;
KEY CPlayerController::Left	 = KEY::A;
KEY CPlayerController::Dash	 = KEY::SPACE;

KEY CPlayerController::Attack = KEY::LBTN;
KEY CPlayerController::Zoom	  = KEY::RBTN;
KEY CPlayerController::Reload = KEY::R;

KEY CPlayerController::Cover = KEY::LSHIFT;
KEY CPlayerController::Skill = KEY::F;
KEY CPlayerController::Jump	 = KEY::SPACE;
KEY CPlayerController::Flip	 = KEY::TAB;

float CPlayerController::Sensitivity = 6.f;

CPlayerController::CPlayerController()
{
}

CPlayerController::~CPlayerController()
{
}

PlayerKeyInfo CPlayerController::GetInfo()
{
	PlayerKeyInfo info;

	info.Front		 = Front;
	info.Right		 = Right;
	info.Back		 = Back;
	info.Left		 = Left;
	info.Dash		 = Dash;
	info.Attack		 = Attack;
	info.Zoom		 = Zoom;
	info.Reload		 = Reload;
	info.Cover		 = Cover;
	info.Skill		 = Skill;
	info.Jump		 = Jump;
	info.Sensitivity = Sensitivity;
	info.Flip		 = Flip;

	return info;
}

void CPlayerController::SetInfo(PlayerKeyInfo _info)
{
	Front		= _info.Front;
	Right		= _info.Right;
	Back		= _info.Back;
	Left		= _info.Left;
	Dash		= _info.Dash;
	Attack		= _info.Attack;
	Zoom		= _info.Zoom;
	Reload		= _info.Reload;
	Cover		= _info.Cover;
	Skill		= _info.Skill;
	Jump		= _info.Jump;
	Flip		= _info.Flip;
	Sensitivity = _info.Sensitivity;
}
