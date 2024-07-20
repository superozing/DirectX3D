﻿#include "pch.h"
#include "CCoverUI.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include "CPlayerScript.h"
#include "CImageUIScript.h"

CCoverUI::CCoverUI()
	: CEventListener((UINT)SCRIPT_TYPE::COVERUI)
{
}

CCoverUI::~CCoverUI()
{
}

void CCoverUI::begin()
{
	AddTarget(SCRIPT_TYPE::PLAYERSCRIPT);
	m_pPlayerScript = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(PlayerName)->GetScript<CPlayerScript>();

	auto child = GetOwner()->GetParent()->GetChild();
	for (auto& obj : child)
	{
		if (obj->GetScript<CImageUIScript>())
		{
			m_pUI = obj;
			break;
		}
	}
}

void CCoverUI::tick()
{
	CEventListener::tick();

	Vec3 vPos	= GetOwner()->GetParent()->Transform()->GetWorldPos();
	Vec3 vScale = GetOwner()->GetParent()->Transform()->GetWorldScale();
	Vec3 vRot	= GetOwner()->GetParent()->Transform()->GetRelativeRotation();

	Vec3 Pos;

	if (m_pPlayerScript->GetCoverType() == CoverType::Normal)
	{
		Pos = vPos;
		Pos.x += vScale.x / 2.f;
		Pos.y += vScale.y / 2.f;
		Pos.z -= vScale.z / 2.f;
		Pos.x -= 50.f;
		Pos.y -= 50.f;
		m_pUI->Transform()->SetRelativePos(Vec3(vScale.x / 2.f - 50.f, vScale.y / 2.f, -vScale.z / 2.f));
		// m_pUI->Transform()->SetRelativeRotation(vRot);
		GamePlayStatic::DrawDebugCube(Pos, Vec3(10.f, 10.f, 10.f), vRot, Vec3(0.f, 1.f, 1.f), false);
		Pos = vPos;
		Pos.x -= vScale.x / 2.f;
		Pos.y += vScale.y / 2.f;
		Pos.z -= vScale.z / 2.f;
		Pos.x += 50.f;
		Pos.y -= 50.f;
		GamePlayStatic::DrawDebugCube(Pos, Vec3(10.f, 10.f, 10.f), vRot, Vec3(0.f, 1.f, 1.f), false);
	}
}
