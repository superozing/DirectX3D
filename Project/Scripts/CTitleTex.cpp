#include "pch.h"
#include "CTitleTex.h"

#include "CTextUI.h"

CTitleTex::CTitleTex()
	: CImageUIScript((UINT)SCRIPT_TYPE::TITLETEX)
{
}

CTitleTex::~CTitleTex()
{
}

void CTitleTex::SetColor(Vec4 _color)
{
	CImageUIScript::SetColor(_color);
	auto& childs = GetOwner()->GetChild();
	for (auto& child : childs)
	{
		auto image = child->GetScript<CImageUIScript>();
		if (image)
		{
			image->SetColor(_color);
		}
		auto text = child->GetScript<CTextUI>();
		if (text)
		{
			text->SetColor(_color);
		}
	}
}

void CTitleTex::Draw(bool _draw)
{
	CImageUIScript::Draw(_draw);

	auto& childs = GetOwner()->GetChild();
	for (auto& child : childs)
	{
		auto image = child->GetScript<CImageUIScript>();
		if (image)
		{
			image->Draw(_draw);
		}
		auto text = child->GetScript<CTextUI>();
		if (text)
		{
			text->Draw(_draw);
		}
	}
}

void CTitleTex::begin()
{
	CImageUIScript::begin();
}

void CTitleTex::tick()
{
	CImageUIScript::tick();
}
