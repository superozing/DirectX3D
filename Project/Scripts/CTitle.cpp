#include "pch.h"
#include "CTitle.h"

#include "CTitleTex.h"

CTitle::CTitle()
	: CScript((UINT)SCRIPT_TYPE::TITLE)
{
}

CTitle::~CTitle()
{
}

void CTitle::begin()
{
	const auto& childs = GetOwner()->GetChild();
	for (auto& child : childs)
	{
		auto script = child->GetScript<CTitleTex>();
		if (script)
		{
			if (child->GetName() == L"Remnants")
			{
				m_pRemnants = script;
			}

			if (child->GetName() == L"FMOD")
			{
				m_pFMod = script;
			}
		}
	}
}

void CTitle::tick()
{
}