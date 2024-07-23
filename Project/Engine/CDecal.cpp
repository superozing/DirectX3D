#include "pch.h"
#include "CDecal.h"

#include "CTransform.h"

CDecal::CDecal()
	: CRenderComponent(COMPONENT_TYPE::DECAL)
	, m_bAsEmissive(false)
	, m_DecalPriority(0)
	, m_RenderDistance(5000.f)
	, m_bCustomAlpha(false)
	, m_fCustomAlpha(1.f)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHcube));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DecalMtrl"), 0);
}

CDecal::~CDecal()
{
}

void CDecal::finaltick()
{
	GamePlayStatic::DrawDebugCube(Transform()->GetWorldMat(), Vec3(0.f, 1.f, 0.f), false);
}

void CDecal::UpdateData()
{
	Transform()->UpdateData();

	// ViewInv * WorldInv
	Matrix mat = g_Transform.matViewInv * Transform()->GetWorldInvMat();
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::MAT_0, mat);

	// AsEmissive
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, m_bAsEmissive);

	// CustomAlpha
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, m_bCustomAlpha);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_fCustomAlpha);

	GetMaterial(0)->UpdateData();
}

void CDecal::render()
{
	if (nullptr == GetMesh() && nullptr == GetMaterial(0))
		return;

	UpdateData();

	GetMesh()->render(0);
}

void CDecal::ChangeMtrl(wstring _MtrlKey)
{
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(_MtrlKey), 0);
}

#define TagAsEmissive "[AsEmissive]"
#define TagDecalPriority "[DecalPriority]"
#define TagRenderDistance "[RenderDistance]"
#define TagUseCustomAlpha "[UseCustomAlpha]"
#define TagCustomAlphaParameter "[CustomAlphaParameter]"
void CDecal::SaveToFile(ofstream& fout)
{
	CRenderComponent::SaveToFile(fout);

	fout << TagAsEmissive << endl;
	fout << m_bAsEmissive << endl;

	fout << TagDecalPriority << endl;
	fout << m_DecalPriority << endl;

	fout << TagRenderDistance << endl;
	fout << m_RenderDistance << endl;

	fout << TagUseCustomAlpha << endl;
	fout << m_bCustomAlpha << endl;

	fout << TagCustomAlphaParameter << endl;
	fout << m_fCustomAlpha << endl;
}

void CDecal::LoadFromFile(ifstream& fin)
{
	CRenderComponent::LoadFromFile(fin);

	Utils::GetLineUntilString(fin, TagAsEmissive);
	fin >> m_bAsEmissive;

	Utils::GetLineUntilString(fin, TagDecalPriority);
	fin >> m_DecalPriority;

	Utils::GetLineUntilString(fin, TagRenderDistance);
	fin >> m_RenderDistance;

	Utils::GetLineUntilString(fin, TagUseCustomAlpha);
	fin >> m_bCustomAlpha;

	Utils::GetLineUntilString(fin, TagCustomAlphaParameter);
	fin >> m_fCustomAlpha;
}
