#include "pch.h"
#include "CAtlasImageUIScript.h"

CAtlasImageUIScript::CAtlasImageUIScript()
	: CImageUIScript((UINT)SCRIPT_TYPE::ATLASIMAGEUISCRIPT)
	, m_vLeftTop()
	, m_vSize(Vec2(1.f, 1.f))
{
	init();
}

CAtlasImageUIScript::CAtlasImageUIScript(const CAtlasImageUIScript& _Origin)
	: CImageUIScript(_Origin)
	, m_vLeftTop(_Origin.m_vLeftTop)
	, m_vSize(_Origin.m_vSize)
{
}

CAtlasImageUIScript::CAtlasImageUIScript(UINT _type)
	: CImageUIScript(_type)
	, m_vLeftTop()
	, m_vSize(Vec2(1.f, 1.f))
{
	init();
}

CAtlasImageUIScript::~CAtlasImageUIScript()
{
}

void CAtlasImageUIScript::Apply()
{
	if (!m_UIImg.Get())
		return;

	Vec2 vAtlasSize = Vec2(m_UIImg->GetWidth(), m_UIImg->GetHeight());

	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_0, m_vLeftTop / vAtlasSize);
	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_1, m_vSize / vAtlasSize);
}

void CAtlasImageUIScript::init()
{
	AppendScriptParam("LeftTop", SCRIPT_PARAM::VEC2, &m_vLeftTop);
	AppendScriptParam("Size", SCRIPT_PARAM::VEC2, &m_vSize);
	// AppendMemberFunction("Apply", SCRIPT_PARAM::FUNC_MEMBER, "", std::bind(&CAtlasImageUIScript::Apply, this));
}

void CAtlasImageUIScript::begin()
{
	CImageUIScript::begin();

	Apply();
}

void CAtlasImageUIScript::tick()
{
	CImageUIScript::tick();
	Apply();
}

#define TagLeftTop "[LeftTop]"
#define TagSize "[Size]"

void CAtlasImageUIScript::SaveToFile(ofstream& fout)
{
	CImageUIScript::SaveToFile(fout);

	fout << TagLeftTop << endl;
	fout << m_vLeftTop << endl;

	fout << TagSize << endl;
	fout << m_vSize << endl;
}

void CAtlasImageUIScript::LoadFromFile(ifstream& fin)
{
	CImageUIScript::LoadFromFile(fin);

	Utils::GetLineUntilString(fin, TagLeftTop);
	fin >> m_vLeftTop;

	Utils::GetLineUntilString(fin, TagSize);
	fin >> m_vSize;
}
