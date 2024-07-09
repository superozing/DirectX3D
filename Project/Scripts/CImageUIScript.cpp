#include "pch.h"
#include "CImageUIScript.h"

CImageUIScript::CImageUIScript()
	: CUIScript((UINT)SCRIPT_TYPE::IMAGEUISCRIPT)
{
}

CImageUIScript::CImageUIScript(const CImageUIScript& _Other)
	: CUIScript(_Other.GetScriptType())
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
	if (m_bAllowBindTexPerFrame && m_UIImg.Get())
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_UIImg);
}

void CImageUIScript::BindUIImgOnTexParam()
{
	if (m_UIImg.Get())
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_UIImg);
	else
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, nullptr);
}

#define TagUIImg "[UIImg]"
#define TagAllowBindTexPerFrame "[AllowBindTexPerFrame]"

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
