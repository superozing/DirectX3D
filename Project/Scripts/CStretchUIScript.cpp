#include "pch.h"
#include "CStretchUIScript.h"

CStretchUIScript::CStretchUIScript()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::STRETCHUISCRIPT)
{
	AppendScriptParam("BaseSize", SCRIPT_PARAM::VEC2, &m_vBaseSize);
}

CStretchUIScript::CStretchUIScript(const CStretchUIScript& _Origin)
	: CAtlasImageUIScript(_Origin)
	, m_vBaseSize(_Origin.m_vBaseSize)
{
}

CStretchUIScript::~CStretchUIScript()
{
}

void CStretchUIScript::begin()
{
	CAtlasImageUIScript::begin();
}

void CStretchUIScript::tick()
{
	CAtlasImageUIScript::tick();

	if (!m_UIImg.Get())
		return;

	Vec2 vAtlasSize = Vec2(m_UIImg->GetWidth(), m_UIImg->GetHeight());
	Vec3 vScale		= Transform()->GetRelativeScale();
	Vec2 vScale2	= Vec2(vScale.x, vScale.y);
	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_2, m_vBaseSize / vScale2);
}

#define TagBaseSize "[BaseSize]"

void CStretchUIScript::SaveToFile(ofstream& fout)
{
	CAtlasImageUIScript::SaveToFile(fout);

	fout << TagBaseSize << endl;
	fout << m_vBaseSize << endl;
}

void CStretchUIScript::LoadFromFile(ifstream& fin)
{
	CAtlasImageUIScript::LoadFromFile(fin);

	Utils::GetLineUntilString(fin, TagBaseSize);
	fin >> m_vBaseSize;
}
