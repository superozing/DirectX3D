#include "pch.h"
#include "CGroundCrackScript.h"

CGroundCrackScript::CGroundCrackScript()
	: CScript((UINT)SCRIPT_TYPE::GROUNDCRACKSCRIPT)
	, m_Pos{}
{
}

CGroundCrackScript::~CGroundCrackScript()
{
}

void CGroundCrackScript::begin()
{
	Transform()->SetRelativePos(m_Pos);

	Decal()->SetUseCustomAlpha(true);
	Decal()->SetCustomAlpha(1.f);
}

void CGroundCrackScript::tick()
{
	float alpha = Decal()->GetCustomAlpha();
	alpha -= DT * 0.3f;
	Decal()->SetCustomAlpha(alpha);

	if (alpha <= 0.f)
		GamePlayStatic::DestroyGameObject(GetOwner());
}