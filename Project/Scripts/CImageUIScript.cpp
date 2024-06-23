#include "pch.h"
#include "CImageUIScript.h"

CImageUIScript::CImageUIScript()
{
}

CImageUIScript::CImageUIScript(const CImageUIScript& _Other)
{
}

CImageUIScript::~CImageUIScript()
{
}

void CImageUIScript::begin()
{
	GetOwner()->MeshRender()->GetDynamicMaterial(0);
}

void CImageUIScript::tick()
{
	if (m_bAllowBindTexPerFrame)
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_UIImg);
}

void CImageUIScript::BindUIImgOnTexParam()
{
	GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_UIImg);
}

#define TagUIImg	"[UIImg]"
#define TagAllowBindTexPerFrame	"[AllowBindTexPerFrame]"

void CImageUIScript::SaveToFile(FILE* _File)
{
}

void CImageUIScript::LoadFromFile(FILE* _File)
{
}

void CImageUIScript::SaveToFile(ofstream& fout)
{
	fout << TagUIImg << endl;
	SaveAssetRef(m_UIImg, fout);

	fout << TagAllowBindTexPerFrame << endl;
	fout << m_bAllowBindTexPerFrame << endl;
}


void CImageUIScript::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagUIImg);
	LoadAssetRef(m_UIImg, fin);

	Utils::GetLineUntilString(fin, TagAllowBindTexPerFrame);
	fin >> m_bAllowBindTexPerFrame;
}
