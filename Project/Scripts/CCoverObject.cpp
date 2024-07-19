#include "pch.h"
#include "CCoverObject.h"

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

	Vec3 vPos	= Transform()->GetRelativePos();
	Vec3 vScale = Transform()->GetRelativeScale();
	Vec3 vRot	= Transform()->GetRelativeRotation();

	Vec3 Pos;
	Pos = vPos;
	Pos.x += vScale.x / 2.f;
	Pos.y += vScale.y / 2.f;
	Pos.z += vScale.z / 2.f;
	Pos.x -= 50.f;
	Pos.y -= 50.f;
	GamePlayStatic::DrawDebugCube(Pos, Vec3(10.f, 10.f, 10.f), vRot, Vec3(0.f, 1.f, 1.f), false);

	Pos = vPos;
	Pos.x -= vScale.x / 2.f;
	Pos.y += vScale.y / 2.f;
	Pos.z += vScale.z / 2.f;
	Pos.x += 50.f;
	Pos.y -= 50.f;
	GamePlayStatic::DrawDebugCube(Pos, Vec3(10.f, 10.f, 10.f), vRot, Vec3(0.f, 1.f, 1.f), false);

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
