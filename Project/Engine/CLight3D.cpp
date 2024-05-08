#include "pch.h"
#include "CLight3D.h"

#include "CRenderMgr.h"
#include "CTransform.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_LightIdx(-1)
{
	SetLightType(LIGHT_TYPE::DIRECTIONAL);
}

CLight3D::~CLight3D()
{
}


void CLight3D::finaltick()
{
	m_Info.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	m_Info.vWorldPos = Transform()->GetWorldPos();

	if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_Info.LightType)
		Transform()->SetRelativeScale(Vec3(m_Info.fRadius * 2.f, m_Info.fRadius * 2.f, m_Info.fRadius * 2.f));

	// 광원을 등록하면서 자신이 구조화 버퍼에서 속한 인덱스 값 가져오기
	m_LightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);

	// 현재 광원의 위치에 DebugRender 요청
	if (m_Info.LightType == (int)LIGHT_TYPE::POINT)
	{
		GamePlayStatic::DrawDebugSphere(m_Info.vWorldPos, m_Info.fRadius, Vec3(0.f, 1.f, 0.1f), true);
	}
	else if (m_Info.LightType == (int)LIGHT_TYPE::SPOT)
	{
		GamePlayStatic::DrawDebugCone(m_Info.vWorldPos, Vec3(m_Info.fRadius, m_Info.fRadius, m_Info.fRadius), Vec3(0.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.1f), true);
	}
}

void CLight3D::render()
{
	m_LightMtrl->SetScalarParam(SCALAR_PARAM::INT_0, m_LightIdx);

	if (LIGHT_TYPE::DIRECTIONAL != (LIGHT_TYPE)m_Info.LightType)
	{
		Matrix matVWInv = g_Transform.matViewInv * Transform()->GetWorldInvMat();
		m_LightMtrl->SetScalarParam(SCALAR_PARAM::MAT_0, matVWInv);
	}
	// 
	Transform()->UpdateData();
	m_LightMtrl->UpdateData();
	m_VolumeMesh->render();
}

void CLight3D::SetLightType(LIGHT_TYPE _type)
{
	m_Info.LightType = (int)_type;

	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_Info.LightType)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
		m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirLightMtrl");
	}

	else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_Info.LightType)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh");
		m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PointLightMtrl");
	}

	else if (LIGHT_TYPE::SPOT == (LIGHT_TYPE)m_Info.LightType)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"ConeMesh");
		m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpotLightMtrl");
	}
}


void CLight3D::SaveToFile(FILE* _File)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight3D::SaveToFile(ofstream& fout)
{
	fout << m_Info << endl;
}

void CLight3D::LoadFromFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight3D::LoadFromFile(ifstream& fin)
{
	fin >> m_Info;
}
