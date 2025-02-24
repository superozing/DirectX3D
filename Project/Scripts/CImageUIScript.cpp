﻿#include "pch.h"
#include "CImageUIScript.h"

CImageUIScript::CImageUIScript()
	: CUIScript((UINT)SCRIPT_TYPE::IMAGEUISCRIPT)
	, m_bDraw(true)
	, m_vBlendColor(Vec4(1.f, 1.f, 1.f, 1.f))
{
	init();
}

CImageUIScript::CImageUIScript(UINT _type)
	: CUIScript(_type)
	, m_bDraw(true)
	, m_vBlendColor(Vec4(1.f, 1.f, 1.f, 1.f))
{
	init();
}

CImageUIScript::CImageUIScript(const CImageUIScript& _Other)
	: CUIScript(_Other)
	, m_bDraw(_Other.m_bDraw)
	, m_UIImg(_Other.m_UIImg)
	, m_vBlendColor(_Other.m_vBlendColor)
{
}

CImageUIScript::~CImageUIScript()
{
}

void CImageUIScript::init()
{
	AppendScriptAsset("Img", &m_UIImg, ASSET_TYPE::TEXTURE);
	AppendScriptParam("Draw", SCRIPT_PARAM::BOOL, &m_bDraw);
	AppendScriptParam("BlendColor", SCRIPT_PARAM::VEC4, &m_vBlendColor);
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
	CUIScript::tick();

	if (m_bAllowBindTexPerFrame && m_UIImg.Get())
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_UIImg);

	if (m_bDraw)
	{
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_UIImg);
		MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, m_vBlendColor);
	}
	else
	{
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, nullptr);
		MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(0.f, 0.f, 0.f, 0.f));
	}
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
#define TagDraw "[Draw]"
#define TagColor "[BlendColor]"

void CImageUIScript::SaveToFile(ofstream& fout)
{
	fout << TagUIImg << endl;
	SaveAssetRef(m_UIImg, fout);

	fout << TagAllowBindTexPerFrame << endl;
	fout << m_bAllowBindTexPerFrame << endl;

	fout << TagDraw << endl;
	fout << m_bDraw << endl;

	fout << TagColor << endl;
	fout << m_vBlendColor << endl;
}

void CImageUIScript::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagUIImg);
	LoadAssetRef(m_UIImg, fin);

	Utils::GetLineUntilString(fin, TagAllowBindTexPerFrame);
	fin >> m_bAllowBindTexPerFrame;

	Utils::GetLineUntilString(fin, TagDraw);
	fin >> m_bDraw;

	Utils::GetLineUntilString(fin, TagColor);
	fin >> m_vBlendColor;
}
