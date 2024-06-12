#include "pch.h"
#include "CBtnUIScript.h"

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
}

void CBtnUIScript::OnHovered()
{
	m_CurImg = m_HoverImg;
	if (m_AllowTexSet && m_CurImg.Get())
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);

}

void CBtnUIScript::OnUnHovered()
{
	m_CurImg = m_NormalImg;
	if (m_AllowTexSet && m_CurImg.Get())
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);

}

void CBtnUIScript::MouseOn()
{
}

void CBtnUIScript::LBtnDown()
{
	m_CurImg = m_PressedImg;
	if (m_AllowTexSet && m_CurImg.Get())
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);

}

void CBtnUIScript::LBtnUp()
{
	m_CurImg = m_NormalImg;
	if (m_AllowTexSet && m_CurImg.Get())
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);

}

void CBtnUIScript::LBtnClicked()
{
	m_CurImg = m_NormalImg;
	if (m_AllowTexSet && m_CurImg.Get())
		GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);

	// CallBack
	if (m_CallBackFunc)
		m_CallBackFunc();

	// Delegate
	if (m_Inst && m_Delegate)
		(m_Inst->*m_Delegate)();
}