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
void CPlayerController::Init()
{
	CPlayerController::Front = KEY::W;
	CPlayerController::Right = KEY::D;
	CPlayerController::Back	 = KEY::S;
	CPlayerController::Left	 = KEY::A;
	CPlayerController::Dash	 = KEY::SPACE;

	CPlayerController::Attack = KEY::LBTN;
	CPlayerController::Zoom	  = KEY::RBTN;
	CPlayerController::Reload = KEY::R;

	CPlayerController::Cover = KEY::LSHIFT;
	CPlayerController::Skill = KEY::F;
	CPlayerController::Jump	 = KEY::SPACE;
	CPlayerController::Flip	 = KEY::TAB;

	CPlayerController::Sensitivity = 6.f;
}

ofstream& operator<<(ofstream& fout, const PlayerKeyInfo& _info)
{
	fout << _info.Front << endl;
	fout << _info.Right << endl;
	fout << _info.Back << endl;
	fout << _info.Left << endl;
	fout << _info.Dash << endl;
	fout << _info.Attack << endl;
	fout << _info.Zoom << endl;
	fout << _info.Reload << endl;
	fout << _info.Cover << endl;
	fout << _info.Skill << endl;
	fout << _info.Jump << endl;
	fout << _info.Flip << endl;
	fout << _info.Sensitivity << endl;

	return fout;
}

ifstream& operator>>(ifstream& fin, PlayerKeyInfo& _info)
{
	int key;
	fin >> key;
	_info.Front = (KEY)key;
	fin >> key;
	_info.Right = (KEY)key;
	fin >> key;
	_info.Back = (KEY)key;
	fin >> key;
	_info.Left = (KEY)key;
	fin >> key;
	_info.Dash = (KEY)key;
	fin >> key;
	_info.Attack = (KEY)key;
	fin >> key;
	_info.Zoom = (KEY)key;
	fin >> key;
	_info.Reload = (KEY)key;
	fin >> key;
	_info.Cover = (KEY)key;
	fin >> key;
	_info.Skill = (KEY)key;
	fin >> key;
	_info.Jump = (KEY)key;
	fin >> key;
	_info.Flip = (KEY)key;
	float fSensitive;
	fin >> fSensitive;
	_info.Sensitivity = fSensitive;

	return fin;
}
