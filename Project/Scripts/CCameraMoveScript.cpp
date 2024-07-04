﻿#include "pch.h"
#include "CCameraMoveScript.h"

CCameraMoveScript::CCameraMoveScript()
	: CScript((UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT)
	, m_CamSpeed(500.f)
	, m_fZoomSpeed(10.f)
{
	AppendScriptParam("ZoomSpeed", SCRIPT_PARAM::FLOAT, &m_fZoomSpeed, 0);
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::tick()
{
	if (KEY_TAP(KEY::P))
	{
		if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		else
		{
			Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
			Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		}
	}

	if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
	{
		MoveOrthographic();
	}

	else
	{
		MovePerspective();
	}

	Vec3 vPos = Transform()->GetRelativePos();

	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	// 줌 인, 줌 아웃
	if (WHEEL_CHECK(WHEEL_UP))
	{
		vPos += DT_ENGINE * m_CamSpeed * vFront;
	}

	if (WHEEL_CHECK(WHEEL_DOWN))
	{
		vPos += DT_ENGINE * m_CamSpeed * -vFront;
	}

	Transform()->SetRelativePos(vPos);
}

void CCameraMoveScript::MoveOrthographic()
{
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::RBTN) && KEY_PRESSED(KEY::W))
	{
		vPos.y += DT_ENGINE * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::RBTN) && KEY_PRESSED(KEY::S))
	{
		vPos.y -= DT_ENGINE * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::RBTN) && KEY_PRESSED(KEY::A))
	{
		vPos.x -= DT_ENGINE * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::RBTN) && KEY_PRESSED(KEY::D))
	{
		vPos.x += DT_ENGINE * m_CamSpeed;
	}

	Transform()->SetRelativePos(vPos);
}

void CCameraMoveScript::MovePerspective()
{
	Vec3 vPos = Transform()->GetRelativePos();

	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vUp	= Transform()->GetWorldDir(DIR_TYPE::UP);

	float camspeed = m_CamSpeed;

	if (KEY_PRESSED(KEY::LSHIFT))
		camspeed *= 5.f;

	if (KEY_PRESSED(KEY::RBTN) && KEY_PRESSED(KEY::W))
	{
		vPos += DT_ENGINE * camspeed * vFront;
	}

	if (KEY_PRESSED(KEY::RBTN) && KEY_PRESSED(KEY::S))
	{
		vPos += DT_ENGINE * camspeed * -vFront;
	}

	if (KEY_PRESSED(KEY::RBTN) && KEY_PRESSED(KEY::A))
	{
		vPos += DT_ENGINE * camspeed * -vRight;
	}

	if (KEY_PRESSED(KEY::RBTN) && KEY_PRESSED(KEY::D))
	{
		vPos += DT_ENGINE * camspeed * vRight;
	}

	if (KEY_PRESSED(KEY::RBTN) && KEY_PRESSED(KEY::Q))
	{
		vPos += DT_ENGINE * m_CamSpeed * -vUp;
	}

	if (KEY_PRESSED(KEY::RBTN) && KEY_PRESSED(KEY::E))
	{
		vPos += DT_ENGINE * m_CamSpeed * vUp;
	}

	Transform()->SetRelativePos(vPos);

	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vDrag = CKeyMgr::GetInst()->GetMouseDrag();
		Vec3 vRot  = Transform()->GetRelativeRotation();
		vRot.y += vDrag.x * DT_ENGINE * XM_PI * 4.f;
		vRot.x += vDrag.y * DT_ENGINE * XM_PI * 4.f;
		Transform()->SetRelativeRotation(vRot);
	}
}
