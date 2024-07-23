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

	if (KEY_TAP(CPlayerController::Cover) && m_bPlayerCover)
	{
		m_pScript->SetCoverType(CoverType::Normal);
		m_bPlayerCover = false;
	}
	else if (KEY_TAP(CPlayerController::Cover) && !m_bPlayerCover)
	{
		SetPlayerCoverRight();
	}
}
