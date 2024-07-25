#include "pch.h"
#include "CDecal.h"

#include "CTransform.h"
#include "CTimeMgr.h"

CDecal::CDecal()
	: CRenderComponent(COMPONENT_TYPE::DECAL)
	, m_bAsEmissive(false)
	, m_DecalPriority(0)
	, m_RenderDistance(5000.f)
	, m_bCustomAlpha(false)
	, m_fCustomAlpha(1.f)
	, m_DecalShape(DecalShape::Rect)
	, m_DecalType(DecalType::Texture)
	, m_fAnimationActionTime(0.f)
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

	m_fAnimationActionTime -= DT;

	if (m_fAnimationActionTime <= 0.f)
		m_fAnimationActionTime = 0.f;
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

	Vec2 DecalInfo = Vec2{(int)m_DecalShape, (int)m_DecalType};
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_0, DecalInfo);

	if (m_fAnimationActionTime <= 0.f && m_fAnimationActionTime != -100.f)
		m_fAnimationActionTime = 0.f;

	// 애니메이션 관련 정보
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, m_AnimationOutlinerColor);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_1, m_AnimationOutsideColor);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_2, m_AnimationInsideColor);

	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, m_fAnimationActionTime);

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

#define TagDecalShape "[DecalShape]"
#define TagDecalType "[DecalType]"
#define TagDecalActionTime "[DecalActionTime]"
#define TagAsEmissive "[AsEmissive]"
#define TagDecalPriority "[DecalPriority]"
#define TagRenderDistance "[RenderDistance]"
#define TagUseCustomAlpha "[UseCustomAlpha]"
#define TagCustomAlphaParameter "[CustomAlphaParameter]"
#define TagAninationOutlinerColor "[Animation Outliner Color]"
#define TagAnimationInsideColor "[Animation Inside Color]"
#define TagAnimationOutsideColor "[Animation Outside Color]"

void CDecal::SaveToFile(ofstream& fout)
{
	CRenderComponent::SaveToFile(fout);

	fout << TagDecalShape << endl;
	fout << (int)m_DecalShape << endl;

	fout << TagDecalType << endl;
	fout << (int)m_DecalType << endl;

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

	fout << TagDecalActionTime << endl;
	fout << m_OriginAnimationActionTime << endl;

	fout << TagAninationOutlinerColor << endl;
	fout << m_AnimationOutlinerColor.x << " " << m_AnimationOutlinerColor.y << " " << m_AnimationOutlinerColor.z << " "
		 << m_AnimationOutlinerColor.w << endl;

	fout << TagAnimationInsideColor << endl;
	fout << m_AnimationInsideColor.x << " " << m_AnimationInsideColor.y << " " << m_AnimationInsideColor.z << " "
		 << m_AnimationInsideColor.w << endl;

	fout << TagAnimationOutsideColor << endl;
	fout << m_AnimationOutsideColor.x << " " << m_AnimationOutsideColor.y << " " << m_AnimationOutsideColor.z << " "
		 << m_AnimationOutsideColor.w << endl;
}

void CDecal::LoadFromFile(ifstream& fin)
{
	CRenderComponent::LoadFromFile(fin);

	int iDecalInfo;

	Utils::GetLineUntilString(fin, TagDecalShape);
	fin >> iDecalInfo;

	m_DecalShape = (DecalShape)iDecalInfo;

	Utils::GetLineUntilString(fin, TagDecalType);
	fin >> iDecalInfo;

	m_DecalType = (DecalType)iDecalInfo;

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

	Utils::GetLineUntilString(fin, TagDecalActionTime);
	fin >> m_OriginAnimationActionTime;

	Vec4 AnimationColor;

	Utils::GetLineUntilString(fin, TagAninationOutlinerColor);
	fin >> AnimationColor.x >> AnimationColor.y >> AnimationColor.z >> AnimationColor.w;
	m_AnimationOutlinerColor = AnimationColor;

	Utils::GetLineUntilString(fin, TagAnimationInsideColor);
	fin >> AnimationColor.x >> AnimationColor.y >> AnimationColor.z >> AnimationColor.w;
	m_AnimationInsideColor = AnimationColor;

	Utils::GetLineUntilString(fin, TagAnimationOutsideColor);
	fin >> AnimationColor.x >> AnimationColor.y >> AnimationColor.z >> AnimationColor.w;
	m_AnimationOutsideColor = AnimationColor;

	SyncTime();
}
