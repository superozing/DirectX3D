#include "pch.h"
#include "CWrapImage.h"

CWrapImage::CWrapImage()
	: CScript((UINT)SCRIPT_TYPE::WRAPIMAGE)
	, m_Mode(WrapMode::BasedOnTexture)
	, WrapSize(0.f, 0.f)
	, OriginTexSize(0.f, 0.f)
	, ToSaveWrapSize(0.f, 0.f)
{
	m_strMode = ToString(magic_enum::enum_name(m_Mode));
	AppendScriptParam("Wrap Mode", SCRIPT_PARAM::STRING, (void*)&m_strMode);
	AppendMemberFunction("ChangeMode", SCRIPT_PARAM::FUNC_MEMBER, "Wrap", std::bind(&CWrapImage::ChangeWrapMode, this));
	AppendScriptParam("Custom Wrap Size", SCRIPT_PARAM::VEC2, (void*)&WrapSize);
}

CWrapImage::~CWrapImage()
{
}

void CWrapImage::begin()
{
	PlaneScale = Vec2(GetOwner()->Transform()->GetRelativeScale().x, GetOwner()->Transform()->GetRelativeScale().y);
	GetOwner()->MeshRender()->GetDynamicMaterial(0);
	OriginTexSize = Vec2(GetOwner()->MeshRender()->GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_0).Get()->GetWidth(),
						 GetOwner()->MeshRender()->GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_0).Get()->GetHeight());
}

void CWrapImage::tick()
{
	m_strMode = ToString(magic_enum::enum_name(m_Mode));

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

void CWrapImage::ChangeWrapMode()
{
	if (m_Mode == WrapMode::BasedOnTexture)
	{
		m_Mode = WrapMode::Custom;
	}
	else
	{
		m_Mode = WrapMode::BasedOnTexture;
	}

	m_strMode = ToString(magic_enum::enum_name(m_Mode));
}

#define TagWrapType "[WrapType]"
#define TagWrapSize "[TagWrapSize]"

void CWrapImage::SaveToFile(ofstream& fout)
{
	fout << TagWrapType << endl;
	fout << magic_enum::enum_name(m_Mode) << endl;

	fout << TagWrapSize << endl;
	fout << (int)WrapSize.x << " " << (int)WrapSize.y << endl;
}

void CWrapImage::LoadFromFile(ifstream& fin)
{
	string strWrapType;

	Utils::GetLineUntilString(fin, TagWrapType);
	getline(fin, strWrapType);

	auto Type = magic_enum::enum_cast<WrapMode>(strWrapType);
	m_Mode	  = Type.value();

	m_strMode = ToString(magic_enum::enum_name(m_Mode));

	Vec2 TempWrapSize;
	Utils::GetLineUntilString(fin, TagWrapSize);
	fin >> TempWrapSize.x >> TempWrapSize.y;

	WrapSize = TempWrapSize;
}
