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

CPlayerController::CPlayerController()
{
}

CPlayerController::~CPlayerController()
{
}
