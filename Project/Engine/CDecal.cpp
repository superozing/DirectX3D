#include "pch.h"
#include "CDecal.h"

#include "CTransform.h"

CDecal::CDecal()
	: CRenderComponent(COMPONENT_TYPE::DECAL)
	, m_bAsEmissive(false)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHcube));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DecalMtrl"), 0);
}

CDecal::~CDecal()
{
}

void CDecal::finaltick()
{
	GamePlayStatic::DrawDebugCube(Transform()->GetWorldMat(), Vec3(0.f, 1.f, 0.f), true);
}

void CDecal::UpdateData()
{
	Transform()->UpdateData();

	// ViewInv * WorldInv
	Matrix mat = g_Transform.matViewInv * Transform()->GetWorldInvMat();
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::MAT_0, mat);

	// AsEmissive
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, m_bAsEmissive);

	GetMaterial(0)->UpdateData();
}

void CDecal::render()
{
	if (nullptr == GetMesh() && nullptr == GetMaterial(0))
		return;

	UpdateData();

	GetMesh()->render(0);
}

#define TagAsEmissive "[AsEmissive]"

void CDecal::SaveToFile(ofstream& fout)
{
	CRenderComponent::SaveToFile(fout);

	fout << TagAsEmissive << endl;
	fout << m_bAsEmissive << endl;
}

void CDecal::LoadFromFile(ifstream& fin)
{
	CRenderComponent::LoadFromFile(fin);

	Utils::GetLineUntilString(fin, TagAsEmissive);
	fin >> m_bAsEmissive;
}
