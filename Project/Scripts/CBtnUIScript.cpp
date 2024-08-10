#include "pch.h"
#include "CBtnUIScript.h"
#include <Engine/CLogMgr.h>

static string strImg = "";

CBtnUIScript::CBtnUIScript()
	: CUIScript((UINT)SCRIPT_TYPE::BTNUISCRIPT)
	, m_NormalImg(nullptr)
	, m_HoverImg(nullptr)
	, m_PressedImg(nullptr)
	, m_CurImg(nullptr)
	, m_CallBackFunc(nullptr)
	, m_Inst(nullptr)
	, m_Delegate(nullptr)
	, m_AllowTexSet(false)
	, m_bDraw(true)
	, m_vNLT(Vec2(0, 0))
	, m_vHLT(Vec2(0, 0))
	, m_vPLT(Vec2(0, 0))
	, m_vNSz(Vec2(0, 0))
	, m_vHSz(Vec2(0, 0))
	, m_vPSz(Vec2(0, 0))
	, m_vNColor(Vec4(255.f, 255.f, 255.f, 255.f))
	, m_vHColor(Vec4(255.f, 255.f, 255.f, 255.f))
	, m_vPColor(Vec4(255.f, 255.f, 255.f, 255.f))

{
	AppendScriptParam("Draw", SCRIPT_PARAM::BOOL, &m_bDraw);
	AppendScriptParam("CurImg", SCRIPT_PARAM::STRING, &strImg);

	AppendScriptAsset("Normal", &m_NormalImg, ASSET_TYPE::TEXTURE);
	AppendScriptParam("Normal LeftTop", SCRIPT_PARAM::VEC2, &m_vNLT);
	AppendScriptParam("Normal Size", SCRIPT_PARAM::VEC2, &m_vNSz);
	AppendScriptParam("Normal Color", SCRIPT_PARAM::VEC4, &m_vNColor);
	AppendScriptAsset("Hovered", &m_HoverImg, ASSET_TYPE::TEXTURE);
	AppendScriptParam("Hovered LeftTop", SCRIPT_PARAM::VEC2, &m_vHLT);
	AppendScriptParam("Hovered Size", SCRIPT_PARAM::VEC2, &m_vHSz);
	AppendScriptParam("Hovered Color", SCRIPT_PARAM::VEC4, &m_vHColor);
	AppendScriptAsset("Pressed", &m_PressedImg, ASSET_TYPE::TEXTURE);
	AppendScriptParam("Pressed LeftTop", SCRIPT_PARAM::VEC2, &m_vPLT);
	AppendScriptParam("Pressed Size", SCRIPT_PARAM::VEC2, &m_vPSz);
	AppendScriptParam("Pressed Color", SCRIPT_PARAM::VEC4, &m_vPColor);
}

CBtnUIScript::CBtnUIScript(const CBtnUIScript& _Other)
	: CUIScript(_Other.GetScriptType())
	, m_NormalImg(_Other.m_NormalImg)
	, m_HoverImg(_Other.m_HoverImg)
	, m_PressedImg(_Other.m_PressedImg)
	, m_CurImg(nullptr)
	, m_CallBackFunc(_Other.m_CallBackFunc)
	, m_Inst(_Other.m_Inst)
	, m_Delegate(_Other.m_Delegate)
	, m_AllowTexSet(true)
	, m_bDraw(true)
{
}

CBtnUIScript::~CBtnUIScript()
{
}

void CBtnUIScript::begin()
{
	if (m_vNSz == Vec2() && m_NormalImg.Get())
		m_vNSz = Vec2(m_NormalImg->GetWidth(), m_NormalImg->GetHeight());

	if (m_vHSz == Vec2() && m_HoverImg.Get())
		m_vHSz = Vec2(m_HoverImg->GetWidth(), m_HoverImg->GetHeight());

	if (m_vPSz == Vec2() && m_PressedImg.Get())
		m_vPSz = Vec2(m_PressedImg->GetWidth(), m_PressedImg->GetHeight());

	m_CurImg  = m_NormalImg;
	m_vCLT	  = m_vNLT;
	m_vCSz	  = m_vNSz;
	m_vCColor = m_vNColor;
	m_State	  = ButtonState::Normal;
	GetOwner()->MeshRender()->GetDynamicMaterial(0);
}

void CBtnUIScript::tick()
{
	CUIScript::tick();

	if (m_CurImg.Get())
		strImg = ToString(m_CurImg->GetKey());

	if (m_State == ButtonState::Normal)
	{
		m_vCLT	  = m_vNLT;
		m_vCSz	  = m_vNSz;
		m_vCColor = m_vNColor;
	}

	if (m_State == ButtonState::Hovered)
	{
		m_vCLT	  = m_vHLT;
		m_vCSz	  = m_vHSz;
		m_vCColor = m_vHColor;
	}

	if (m_State == ButtonState::Pressed)
	{
		m_vCLT	  = m_vPLT;
		m_vCSz	  = m_vPSz;
		m_vCColor = m_vPColor;
	}

	if (m_bDraw && m_CurImg.Get())
	{
		Vec2 vAtlasSize = Vec2(m_CurImg->GetWidth(), m_CurImg->GetHeight());
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);
		MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, m_vCColor / 255.f);
		MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_0, m_vCLT / vAtlasSize);
		MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_1, m_vCSz / vAtlasSize);
	}
	else
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, nullptr);
}

void CBtnUIScript::OnHovered()
{
	m_CurImg  = m_HoverImg;
	m_vCLT	  = m_vHLT;
	m_vCSz	  = m_vHSz;
	m_vCColor = m_vHColor;
	m_State	  = ButtonState::Hovered;
}

void CBtnUIScript::OnUnHovered()
{
	m_CurImg  = m_NormalImg;
	m_vCLT	  = m_vNLT;
	m_vCSz	  = m_vNSz;
	m_vCColor = m_vNColor;
	m_State	  = ButtonState::Normal;
}

void CBtnUIScript::MouseOn()
{
}

void CBtnUIScript::LBtnDown()
{
	m_CurImg  = m_PressedImg;
	m_vCLT	  = m_vPLT;
	m_vCSz	  = m_vPSz;
	m_vCColor = m_vPColor;
	m_State	  = ButtonState::Pressed;
}

void CBtnUIScript::LBtnUp()
{
	m_CurImg  = m_NormalImg;
	m_vCLT	  = m_vNLT;
	m_vCSz	  = m_vNSz;
	m_vCColor = m_vNColor;
	m_State	  = ButtonState::Normal;
}

void CBtnUIScript::LBtnClicked()
{
	m_CurImg  = m_NormalImg;
	m_vCLT	  = m_vNLT;
	m_vCSz	  = m_vNSz;
	m_vCColor = m_vNColor;
	m_State	  = ButtonState::Normal;

	if (m_AllowCallFunc)
	{
		// CallBack
		if (m_CallBackFunc)
			m_CallBackFunc();

		// Delegate
		if (m_Inst && m_Delegate)
			(m_Inst->*m_Delegate)();
	}
}

void CBtnUIScript::SaveToFile(FILE* _File)
{
}

void CBtnUIScript::LoadFromFile(FILE* _File)
{
}

#define TagNormalImg "[NormalImg]"
#define TagLeftTop "[LeftTop]"
#define TagSize "[Size]"
#define TagPressedImg "[PressedImg]"
#define TagHoverImg "[HoverImg]"
#define TagAllowTexSet "[AllowTexSet]"

// 콜백은 어떻게 저장해야 할까?

void CBtnUIScript::SaveToFile(ofstream& fout)
{
	fout << TagNormalImg << endl;
	SaveAssetRef(m_NormalImg, fout);

	fout << TagLeftTop << endl;
	fout << m_vNLT << endl;

	fout << TagSize << endl;
	fout << m_vNSz << endl;

	fout << TagPressedImg << endl;
	SaveAssetRef(m_PressedImg, fout);

	fout << TagLeftTop << endl;
	fout << m_vPLT << endl;

	fout << TagSize << endl;
	fout << m_vPSz << endl;

	fout << TagHoverImg << endl;
	SaveAssetRef(m_HoverImg, fout);

	fout << TagLeftTop << endl;
	fout << m_vHLT << endl;

	fout << TagSize << endl;
	fout << m_vHSz << endl;

	fout << TagAllowTexSet << endl;
	fout << m_AllowTexSet << endl;
}

void CBtnUIScript::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagNormalImg);
	LoadAssetRef(m_NormalImg, fin);

	Utils::GetLineUntilString(fin, TagLeftTop);
	fin >> m_vNLT;

	Utils::GetLineUntilString(fin, TagSize);
	fin >> m_vNSz;

	Utils::GetLineUntilString(fin, TagPressedImg);
	LoadAssetRef(m_PressedImg, fin);

	Utils::GetLineUntilString(fin, TagLeftTop);
	fin >> m_vPLT;

	Utils::GetLineUntilString(fin, TagSize);
	fin >> m_vPSz;

	Utils::GetLineUntilString(fin, TagHoverImg);
	LoadAssetRef(m_HoverImg, fin);

	Utils::GetLineUntilString(fin, TagLeftTop);
	fin >> m_vHLT;

	Utils::GetLineUntilString(fin, TagSize);
	fin >> m_vHSz;

	Utils::GetLineUntilString(fin, TagAllowTexSet);
	fin >> m_AllowTexSet;
}
