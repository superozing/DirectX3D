#include "pch.h"
#include "CLaserSetting.h"

#include <Engine\CMaterial.h>
#include <Engine\CAssetMgr.h>

#include <Engine\CGameObject.h>

CLaserSetting::CLaserSetting()
	: CScript((UINT)SCRIPT_TYPE::LASERSETTING)
	, LaserColor(0.f, 0.f, 0.f, 1.f)
	, LaserFrontDir(0.0f, 0.f, 0.f, 0.f)
	, fLaserLength(100.f)
	, fAlpha(1.f)
	, fWidth(1.f)
{
	AppendScriptParam("Laser Color", SCRIPT_PARAM::COLOR, &LaserColor);
	AppendScriptParam("Laser FrontDir", SCRIPT_PARAM::VEC4, &LaserFrontDir);

	AppendScriptParam("Laser Legnth", SCRIPT_PARAM::FLOAT, &fLaserLength);
	AppendScriptParam("Laser Alpha", SCRIPT_PARAM::FLOAT, &fAlpha);
	AppendScriptParam("Laser Width", SCRIPT_PARAM::FLOAT, &fWidth);
}

CLaserSetting::~CLaserSetting()
{
}

void CLaserSetting::begin()
{

	pLaserMtrl = GetOwner()->GetRenderComponent()->GetDynamicMaterial(0);

	pLaserMtrl.Get()->SetScalarParam(SCALAR_PARAM::VEC4_1, &LaserColor);
	pLaserMtrl.Get()->SetScalarParam(SCALAR_PARAM::VEC4_0, &LaserFrontDir);

	pLaserMtrl.Get()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLaserLength);
	pLaserMtrl.Get()->SetScalarParam(SCALAR_PARAM::FLOAT_1, &fAlpha);
	pLaserMtrl.Get()->SetScalarParam(SCALAR_PARAM::FLOAT_2, &fWidth);
}

void CLaserSetting::tick()
{
	pLaserMtrl.Get()->SetScalarParam(SCALAR_PARAM::VEC4_1, &LaserColor);
	pLaserMtrl.Get()->SetScalarParam(SCALAR_PARAM::VEC4_0, &LaserFrontDir);

	pLaserMtrl.Get()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLaserLength);
	pLaserMtrl.Get()->SetScalarParam(SCALAR_PARAM::FLOAT_1, &fAlpha);
	pLaserMtrl.Get()->SetScalarParam(SCALAR_PARAM::FLOAT_2, &fWidth);
}

void CLaserSetting::SaveToFile(ofstream& fout)
{
}

void CLaserSetting::LoadFromFile(ifstream& fin)
{
}
