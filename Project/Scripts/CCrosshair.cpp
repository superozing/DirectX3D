#include "pch.h"
#include "CCrosshair.h"

#include <Engine/CAssetMgr.h>

#include <Engine/CPhysXMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CRandomMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CPlayerScript.h"
#include "CShootingSystemScript.h"

CCrosshair::CCrosshair()
	: CScript((UINT)SCRIPT_TYPE::CROSSHAIR)
	, m_CrosshairColor(Vec4(255, 255, 255, 255))
	, m_fSpreadRatio(0.2)
	, m_fBarThikness(1)
	, m_fLength(5)
{
	AppendScriptParam("Spread Ratio", SCRIPT_PARAM::FLOAT, &m_fSpreadRatio, 0.f, 0.f);
	AppendScriptParam("Color", SCRIPT_PARAM::VEC4, &m_CrosshairColor);
	AppendScriptParam("Bar Thikness", SCRIPT_PARAM::FLOAT, &m_fBarThikness);
	AppendScriptParam("Length", SCRIPT_PARAM::FLOAT, &m_fLength);
}

CCrosshair::~CCrosshair()
{
}

void CCrosshair::begin()
{
	m_pPlayer		= CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(PlayerName);
	m_pPlayerScript = m_pPlayer->GetScript<CPlayerScript>();
}

void CCrosshair::tick()
{
	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_fSpreadRatio);
	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, m_fBarThikness);
	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_2, m_fLength);
	MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, m_CrosshairColor);
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, nullptr);

	// switch 사용 고려할 필요 있어요.
	// LAYER _layer = m_pShootingSystem->GetMainCamAimLayer();
	// if (_layer == LAYER::LAYER_WALL)
	//{
	//	if (m_pPlayerScript->GetCoverType() != CoverType::Normal)
	//	{
	//		SetCrosshairColor(Vec4(255.f, 0.f, 0.f, 255.f));
	//	}
	//}
	// else
	//{
	//	SetCrosshairColor(Vec4(255.f, 255.f, 255.f, 255.f));
	//}
}

#define TagBarThikness "[Bar Thikness]"
#define TagBarLength "[Bar Length]"
#define TagColor "[Bar Color]"

void CCrosshair::SaveToFile(FILE* _File)
{
}

void CCrosshair::SaveToFile(ofstream& fout)
{
	fout << TagBarThikness << endl;
	fout << m_fBarThikness << endl;

	fout << TagBarLength << endl;
	fout << m_fLength << endl;

	fout << TagColor << endl;
	fout << m_CrosshairColor << endl;
}

void CCrosshair::LoadFromFile(FILE* _File)
{
}

void CCrosshair::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagBarThikness);
	fin >> m_fBarThikness;

	Utils::GetLineUntilString(fin, TagBarLength);
	fin >> m_fLength;

	Utils::GetLineUntilString(fin, TagColor);
	fin >> m_CrosshairColor;
}
