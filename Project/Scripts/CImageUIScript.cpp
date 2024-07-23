#include "pch.h"
#include "CImageUIScript.h"

CImageUIScript::CImageUIScript()
	: CUIScript((UINT)SCRIPT_TYPE::IMAGEUISCRIPT)
{
	AppendScriptAsset("Img", &m_UIImg, ASSET_TYPE::TEXTURE);
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
	if (!MeshRender())
	{
		wstring str;
		str += GetOwner()->GetName();
		str += L"의 매쉬 렌더가 없습니다.";
		MessageBox(nullptr, str.c_str(), L"ImageUI Script", 0);

		return;
	}
	else if (!MeshRender()->GetMaterial(0).Get())
	{
		wstring str;
		str += GetOwner()->GetName();
		str += L" 의 머터리얼이 없습니다.";
		MessageBox(nullptr, str.c_str(), L"ImageUIScript", 0);
		return;
	}
	// 다이나믹 UI 일 때 PointMesh가 아닌 경우
	else if (MeshRender()->GetMaterial(0)->GetShader()->GetKey() == GRPSHADERDynamicUIShader)
	{
		if (MeshRender()->GetMesh()->GetKey() != MESHpoint)
		{
			wstring str;
			str += GetOwner()->GetName();
			str += L" ImageUI Script";
			MessageBox(nullptr, L"다이나믹 UI는 포인트 매쉬를 사용해야 합니다.", str.c_str(), 0);
			return;
		}
	}
	// 스태틱 UI 일 때 RectMesh가 아닌 경우
	else if (MeshRender()->GetMaterial(0)->GetShader()->GetKey() == GRPSHADERStaticUIShader)
	{
		if (MeshRender()->GetMesh()->GetKey() != MESHrect)
		{
			wstring str;
			str += GetOwner()->GetName();
			str += L" ImageUI Script";
			MessageBox(nullptr, L"다이나믹 UI는 포인트 매쉬를 사용해야 합니다.", str.c_str(), 0);
			return;
		}
	}

	GetOwner()->MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_UIImg);
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
