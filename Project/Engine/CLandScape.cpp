#include "pch.h"
#include "CLandScape.h"

#include "CTransform.h"

CLandScape::CLandScape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_FaceX(64)
	, m_FaceZ(64)
{
	Init();
}

CLandScape::~CLandScape()
{
}

void CLandScape::finaltick()
{
}

void CLandScape::render()
{
	UpdateData();

	GetMesh()->render();
}

void CLandScape::UpdateData()
{
	Transform()->UpdateData();

	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, m_FaceX);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, m_FaceZ);
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_HeightMapTex);

	GetMaterial()->UpdateData();
}

#define TagFaceX "[FaceX]"
#define TagFaceZ "[FaceZ]"

void CLandScape::SaveToFile(ofstream& fout)
{
	fout << TagFaceX << endl;
	fout << m_FaceX << endl;

	fout << TagFaceZ << endl;
	fout << m_FaceZ << endl;
}

void CLandScape::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagFaceX);
	fin >> m_FaceX;

	Utils::GetLineUntilString(fin, TagFaceZ);
	fin >> m_FaceZ;
}

void CLandScape::SetLandScapeFace(UINT face, bool bXaxis)
{
	if (bXaxis)
	{
		m_FaceX = face;
	}
	else
	{
		m_FaceZ = face;
	}
}