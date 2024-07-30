#include "pch.h"
#include "CDigitUI.h"

#include "CImageUIScript.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CDigitUI::CDigitUI()
	: CImageUIScript((UINT)SCRIPT_TYPE::DIGITUI)
{
	AppendScriptParam("Num", SCRIPT_PARAM::INT, &m_iDigit);
}

CDigitUI::CDigitUI(const CDigitUI& _Origin)
	: CImageUIScript(_Origin)
	, m_iDigit(_Origin.m_iDigit)
{
	AppendScriptParam("Num", SCRIPT_PARAM::INT, &m_iDigit);
}

CDigitUI::~CDigitUI()
{
}

void CDigitUI::begin()
{
	CImageUIScript::begin();
}

void CDigitUI::tick()
{
	CImageUIScript::tick();

	// 1. 자릿수 계산
	int digit = to_string(m_iDigit).size();

	// 2. 자릿수에 따라서 x scale 늘리기
	Transform()->SetRelativeScale(Vec3(32.f * digit, 32.f, 1.f));

	// 3. 자릿수를 텍스쳐 레지스터에 바인딩
	GetRenderComponent()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, digit);

	// 4. 데미지를 텍스쳐 레지스터에 바인딩
	GetRenderComponent()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, m_iDigit);
}

#define TagNum "[DigitNum]"

void CDigitUI::SaveToFile(ofstream& fout)
{
	CImageUIScript::SaveToFile(fout);

	fout << TagNum << endl;
	fout << m_iDigit << endl;
}

void CDigitUI::LoadFromFile(ifstream& fin)
{
	CImageUIScript::LoadFromFile(fin);

	Utils::GetLineUntilString(fin, TagNum);
	fin >> m_iDigit;
}
