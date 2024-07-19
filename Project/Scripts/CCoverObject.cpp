#include "pch.h"
#include "CCoverObject.h"

#include "CPlayerController.h"
#include "CPlayerScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

CCoverObject::CCoverObject()
	: CEventListener((UINT)SCRIPT_TYPE::COVEROBJECT)
{
}

CCoverObject::~CCoverObject()
{
}

void CCoverObject::begin()
{
}

void CCoverObject::tick()
{
	CEventListener::tick();

	Vec3 vPos	= GetOwner()->GetParent()->Transform()->GetWorldPos();
	Vec3 vScale = GetOwner()->GetParent()->Transform()->GetWorldScale();
	Vec3 vRot	= GetOwner()->GetParent()->Transform()->GetRelativeRotation();

	Vec3 Pos;
	// Pos = vPos;
	// Pos.x += vScale.x / 2.f;
	// Pos.y += vScale.y / 2.f;
	// Pos.z += vScale.z / 2.f;
	// Pos.x -= 50.f;
	// Pos.y -= 50.f;
	// GamePlayStatic::DrawDebugCube(Pos, Vec3(10.f, 10.f, 10.f), vRot, Vec3(0.f, 1.f, 1.f), false);

	// Pos = vPos;
	// Pos.x -= vScale.x / 2.f;
	// Pos.y += vScale.y / 2.f;
	// Pos.z += vScale.z / 2.f;
	// Pos.x += 50.f;
	// Pos.y -= 50.f;
	// GamePlayStatic::DrawDebugCube(Pos, Vec3(10.f, 10.f, 10.f), vRot, Vec3(0.f, 1.f, 1.f), false);

	// 플레이어가 범위 안에 들어왔을 때
	// if (HasTargets())
	//{
	auto player = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(PlayerName);
	auto script = player->GetScript<CPlayerScript>();

	if (script->IsRight())
	{
		Pos = vPos;
		Pos.x += vScale.x / 2.f;
		Pos.y += vScale.y / 2.f;
		Pos.z -= vScale.z / 2.f;
		Pos.x -= 50.f;
		Pos.y -= 50.f;
		GamePlayStatic::DrawDebugCube(Pos, Vec3(10.f, 10.f, 10.f), vRot, Vec3(0.f, 1.f, 1.f), false);

		if (KEY_TAP(CPlayerController::Cover) || KEY_PRESSED(CPlayerController::Cover))
		{
			Vec3 vPlayerPos = player->Transform()->GetRelativeScale();
			script->SetCoverType(CoverType::Kneel);
			Pos.x += 25.f;
			Pos.y = vPos.y;
			Pos.y -= vScale.y / 2.f;
			Pos.z = vPos.z;
			Pos.z -= vPlayerPos.z;

			player->Transform()->SetRelativePos(Pos);
			player->Transform()->SetRelativeRotation(vRot);
			player->DeleteComponent(COMPONENT_TYPE::PHYSX);
		}
		//}
		// else
		//{
		Pos = vPos;
		Pos.x -= vScale.x / 2.f;
		Pos.y += vScale.y / 2.f;
		Pos.z -= vScale.z / 2.f;
		Pos.x += 50.f;
		Pos.y -= 50.f;
		GamePlayStatic::DrawDebugCube(Pos, Vec3(10.f, 10.f, 10.f), vRot, Vec3(0.f, 1.f, 1.f), false);

		//	if (KEY_TAP(CPlayerController::Cover) || KEY_PRESSED(CPlayerController::Cover))
		//	{
		//		Vec3 vPlayerPos = player->Transform()->GetRelativeScale();
		//		script->SetCoverType(CoverType::Kneel);
		//		Pos.x += 25.f;
		//		Pos.y = vPos.y;
		//		Pos.y -= vScale.y / 2.f;
		//		Pos.z = vPos.z;
		//		Pos.z -= vPlayerPos.z;

		//		player->Transform()->SetRelativePos(Pos);
		//		player->Transform()->SetRelativeRotation(vRot);
		//		player->DeleteComponent(COMPONENT_TYPE::PHYSX);
		//	}
		//}
	}
}
