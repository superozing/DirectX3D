﻿#include "pch.h"
#include "CLight3D.h"

#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCamera.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_LightIdx(-1)
{
	SetLightType(LIGHT_TYPE::DIRECTIONAL);

	// 광원 시점으로 물체를 찍기 위한 카메라
	m_CamObj = new CGameObject;
	m_CamObj->AddComponent(new CTransform);
	m_CamObj->AddComponent(new CCamera);

	// 방향성 광원 기준, 일직선으로 물체를 투영해서 깊이를 그려야 함
	m_CamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	m_CamObj->Camera()->SetFar(100000);
	m_CamObj->Camera()->LayerCheckAll();
	m_CamObj->Camera()->LayerCheck(31, false);
	m_CamObj->Camera()->SetWidth(10000);
	m_CamObj->Camera()->SetAspectRatio(1.f);
}

CLight3D::CLight3D(const CLight3D& _Other)
	: CComponent(_Other)
	, m_Info(_Other.m_Info)
	, m_LightIdx(_Other.m_LightIdx)
	, m_VolumeMesh(_Other.m_VolumeMesh)
	, m_LightMtrl(_Other.m_LightMtrl)
{
	m_CamObj = (_Other.m_CamObj->Clone());
}

CLight3D::~CLight3D()
{
	if (m_CamObj)
		delete m_CamObj;
}

void CLight3D::finaltick()
{
	if (LIGHT_TYPE::SPOT != (LIGHT_TYPE)m_Info.LightType)
		m_Info.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	m_Info.vWorldPos = Transform()->GetWorldPos();

	if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_Info.LightType)
		Transform()->SetRelativeScale(Vec3(m_Info.fRadius * 2.f, m_Info.fRadius * 2.f, m_Info.fRadius * 2.f));
	else if (LIGHT_TYPE::SPOT == (LIGHT_TYPE)m_Info.LightType)
	{
		float fHalfAngle = m_Info.fAngle / 2.f;
		float fRange	 = m_Info.fRadius * tanf(fHalfAngle);
		Transform()->SetRelativeScale(Vec3(fRange * 2.f, fRange * 2.f, m_Info.fRadius));
		Transform()->SetDir(m_Info.vWorldDir);
	}

	// 광원을 등록하면서 자신이 구조화 버퍼에서 속한 인덱스 값 가져오기
	m_LightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);

	// 현재 광원의 위치에 DebugRender 요청
	if (m_Info.LightType == (int)LIGHT_TYPE::POINT)
	{
		GamePlayStatic::DrawDebugSphere(m_Info.vWorldPos, m_Info.fRadius, Vec3(0.f, 1.f, 0.1f), true);
	}
	else if (m_Info.LightType == (int)LIGHT_TYPE::SPOT)
	{
		float fHalfAngle = m_Info.fAngle / 2.f;
		float fRange	 = m_Info.fRadius * tanf(fHalfAngle);
		GamePlayStatic::DrawDebugCone(m_Info.vWorldPos, Vec3(fRange * 2.f, fRange * 2.f, m_Info.fRadius),
									  Transform()->GetRelativeRotation(), Vec3(0.f, 1.f, 0.1f), true);
	}
}

void CLight3D::render()
{
	m_LightMtrl->SetScalarParam(SCALAR_PARAM::INT_0, m_LightIdx);
	m_LightMtrl->SetScalarParam(SCALAR_PARAM::BOOL_0, CRenderMgr::GetInst()->IsPCFShadow());

	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_Info.LightType)
	{
		Matrix matLightVP = m_CamObj->Camera()->GetViewMat() * m_CamObj->Camera()->GetProjMat();
		m_LightMtrl->SetScalarParam(SCALAR_PARAM::MAT_1, matLightVP);
	}

	if (LIGHT_TYPE::DIRECTIONAL != (LIGHT_TYPE)m_Info.LightType)
	{
		Matrix matVWInv = g_Transform.matViewInv * Transform()->GetWorldInvMat();
		m_LightMtrl->SetScalarParam(SCALAR_PARAM::MAT_0, matVWInv);
	}

	Transform()->UpdateData();
	m_LightMtrl->UpdateData();
	m_VolumeMesh->render(0);
}

void CLight3D::render_shadowdepth()
{
	if (LIGHT_TYPE::DIRECTIONAL != GetLightType())
		return;

	// 광원의 위치값을 CameraObject 에 갱신시킨다.
	m_CamObj->Transform()->SetRelativePos(Transform()->GetWorldPos());
	m_CamObj->Transform()->SetDir(Transform()->GetWorldDir(DIR_TYPE::FRONT));

	m_CamObj->Transform()->finaltick();
	m_CamObj->Camera()->finaltick();

	m_CamObj->Camera()->SortShadowMapObject();
	m_CamObj->Camera()->render_shadowmap();
}

void CLight3D::SetLightType(LIGHT_TYPE _type)
{
	m_Info.LightType = (int)_type;

	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_Info.LightType)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect);
		m_LightMtrl	 = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirLightMtrl");
	}

	else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_Info.LightType)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(MESHsphere);
		m_LightMtrl	 = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PointLightMtrl");
	}

	else if (LIGHT_TYPE::SPOT == (LIGHT_TYPE)m_Info.LightType)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(MESHcone);
		m_LightMtrl	 = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpotLightMtrl");
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
