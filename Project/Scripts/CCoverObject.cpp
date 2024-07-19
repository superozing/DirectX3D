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

void CCoverObject::SetPlayerCoverRight()
{
	Vec3 vPos	= GetOwner()->GetParent()->Transform()->GetWorldPos();
	Vec3 vScale = GetOwner()->GetParent()->Transform()->GetWorldScale();
	Vec3 vRot	= GetOwner()->GetParent()->Transform()->GetRelativeRotation();

	Vec3 vPlayerScale = m_pPlayer->Transform()->GetRelativeScale();
	m_pScript->SetCoverType(CoverType::Kneel);
	vPos.x += vScale.x / 2.f;
	vPos.y -= vScale.y / 2.f;
	vPos.z -= vScale.z / 2.f;

	vPos.x -= vPlayerScale.x / 4.f;
	// vPos.y -= vPlayerScale.x / 2.f;
	vPos.z -= vPlayerScale.z / 2.f;

	m_pPlayer->Transform()->SetRelativePos(vPos);
	m_pPlayer->Transform()->SetRelativeRotation(vRot);

	m_bPlayerCover = true;

	// TODO : 피직스 디텍트만 가능해지면 제거해야 함
	m_pPlayer->DeleteComponent(COMPONENT_TYPE::PHYSX);
}

void CCoverObject::begin()
{
	m_pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(PlayerName);
	m_pScript = m_pPlayer->GetScript<CPlayerScript>();
}

void CCoverObject::tick()
{
	CEventListener::tick();

	Vec3 vPos	= GetOwner()->GetParent()->Transform()->GetWorldPos();
	Vec3 vScale = GetOwner()->GetParent()->Transform()->GetWorldScale();
	Vec3 vRot	= GetOwner()->GetParent()->Transform()->GetRelativeRotation();

	Vec3 Pos;

	// 플레이어가 범위 안에 들어왔을 때
	// if (HasTargets())
	//{
	auto player = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(PlayerName);
	auto script = player->GetScript<CPlayerScript>();
	if (KEY_TAP(CPlayerController::Cover) && m_bPlayerCover)
	{
		script->SetCoverType(CoverType::Normal);
		m_bPlayerCover = false;
	}
	else if (KEY_TAP(CPlayerController::Cover) && !m_bPlayerCover)
	{
		SetPlayerCoverRight();
	}

	Pos = vPos;
	Pos.x += vScale.x / 2.f;
	Pos.y += vScale.y / 2.f;
	Pos.z -= vScale.z / 2.f;
	Pos.x -= 50.f;
	Pos.y -= 50.f;
	GamePlayStatic::DrawDebugCube(Pos, Vec3(10.f, 10.f, 10.f), vRot, Vec3(0.f, 1.f, 1.f), false);
	Pos = vPos;
	Pos.x -= vScale.x / 2.f;
	Pos.y += vScale.y / 2.f;
	Pos.z -= vScale.z / 2.f;
	Pos.x += 50.f;
	Pos.y -= 50.f;
	GamePlayStatic::DrawDebugCube(Pos, Vec3(10.f, 10.f, 10.f), vRot, Vec3(0.f, 1.f, 1.f), false);
}
