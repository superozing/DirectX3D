#include "pch.h"
#include "CWallScript.h"

CWallScript::CWallScript()
	: CScript((UINT)SCRIPT_TYPE::WALLSCRIPT)
	, m_Mode(WrapMode::BasedOnTexture)
	, WrapSize(0.f, 0.f)
	, OriginTexSize(0.f, 0.f)
{
}

CWallScript::~CWallScript()
{
}

static string WrapMode = "";

void CWallScript::begin()
{
	PlaneScale = Vec2(GetOwner()->Transform()->GetRelativeScale().x, GetOwner()->Transform()->GetRelativeScale().y);

	string strMode = ToString(magic_enum::enum_name(m_Mode));

	AppendScriptParam("##Wrap Mode", SCRIPT_PARAM::STRING, (void*)&WrapMode);
	AppendMemberFunction("ChangeMode", SCRIPT_PARAM::FUNC_MEMBER, "Wrap",
						 std::bind(&CWallScript::ChangeWrapMode, this));

	AppendScriptParam("Custom Wrap Size", SCRIPT_PARAM::VEC2, (void*)&WrapSize);

	OriginTexSize = Vec2(GetOwner()->MeshRender()->GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_0).Get()->GetWidth(),
						 GetOwner()->MeshRender()->GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_0).Get()->GetHeight());
}

void CWallScript::tick()
{
	WrapMode = ToString(magic_enum::enum_name(m_Mode));

	Vec2 ObjPlaneScale =
		Vec2(GetOwner()->Transform()->GetRelativeScale().x, GetOwner()->Transform()->GetRelativeScale().y);

	GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_1, ObjPlaneScale);
	PlaneScale = ObjPlaneScale;

	// 매핑시키고 싶은 texture의 좌표를 조정하고 싶을때
	if (m_Mode == WrapMode::Custom)
	{
		GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_0, WrapSize);
	}
	else
	{
		GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_0, OriginTexSize);
	}
}

void CWallScript::ChangeWrapMode()
{
	if (m_Mode == WrapMode::BasedOnTexture)
		m_Mode = WrapMode::Custom;
	else
		m_Mode = WrapMode::BasedOnTexture;
}