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
{
	AppendScriptParam("Draw", SCRIPT_PARAM::BOOL, &m_bDraw);
	AppendScriptAsset("Normal", &m_NormalImg, ASSET_TYPE::TEXTURE);
	AppendScriptParam("CurImg", SCRIPT_PARAM::STRING, &strImg);
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
{
}

CBtnUIScript::~CBtnUIScript()
{
}

void CBtnUIScript::begin()
{
	m_CurImg = m_NormalImg;
	GetOwner()->MeshRender()->GetDynamicMaterial(0);
}

void CBtnUIScript::tick()
{
	CUIScript::tick();
	strImg = ToString(m_CurImg->GetKey());

	if (m_bDraw)
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);
	else
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, nullptr);
}

void CBtnUIScript::OnHovered()
{
	m_CurImg = m_HoverImg;
}

void CBtnUIScript::OnUnHovered()
{
	m_CurImg = m_NormalImg;
}

void CBtnUIScript::MouseOn()
{
}

void CBtnUIScript::LBtnDown()
{
	m_CurImg = m_PressedImg;
}

void CBtnUIScript::LBtnUp()
{
	m_CurImg = m_NormalImg;
}

void CBtnUIScript::LBtnClicked()
{
	m_CurImg = m_NormalImg;

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
#define TagPressedImg "[PressedImg]"
#define TagHoverImg "[HoverImg]"
#define TagAllowTexSet "[AllowTexSet]"

// 콜백은 어떻게 저장해야 할까?

void CBtnUIScript::SaveToFile(ofstream& fout)
{
	fout << TagNormalImg << endl;
	SaveAssetRef(m_NormalImg, fout);

	fout << TagPressedImg << endl;
	SaveAssetRef(m_PressedImg, fout);

	fout << TagHoverImg << endl;
	SaveAssetRef(m_HoverImg, fout);

	fout << TagAllowTexSet << endl;
	fout << m_AllowTexSet << endl;
}

void CBtnUIScript::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagNormalImg);
	LoadAssetRef(m_NormalImg, fin);

	Utils::GetLineUntilString(fin, TagPressedImg);
	LoadAssetRef(m_PressedImg, fin);

	Utils::GetLineUntilString(fin, TagHoverImg);
	LoadAssetRef(m_HoverImg, fin);

	Utils::GetLineUntilString(fin, TagAllowTexSet);
	fin >> m_AllowTexSet;
}
