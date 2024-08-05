#include "pch.h"
#include "CDirectionalLight.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CDirectionalLight::CDirectionalLight()
	: CScript((UINT)SCRIPT_TYPE::DIRECTIONALLIGHT)
	, m_fDistance(300.f)
	, m_Player(nullptr)
{
	AppendScriptParam("Distance", SCRIPT_PARAM::FLOAT, &m_fDistance);
}

CDirectionalLight::~CDirectionalLight()
{
}

void CDirectionalLight::begin()
{
	m_Player = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(PlayerName);
	// m_fDistance = Vec3::Distance(m_Player->Transform()->GetRelativePos(), Transform()->GetRelativePos());
}

void CDirectionalLight::tick()
{
	Vec3 vPos = m_Player->Transform()->GetRelativePos();
	vPos += -Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_fDistance;
	Transform()->SetRelativePos(vPos);
}

#define TagDistance "[LightDistance]"

void CDirectionalLight::SaveToFile(ofstream& fout)
{
	fout << TagDistance << endl;
	fout << m_fDistance;
}

void CDirectionalLight::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagDistance);
	fin >> m_fDistance;
}
