#include "pch.h"
#include "CCrosshair.h"

#include <Engine/CAssetMgr.h>

#include "CImageUIScript.h"
#include "CPanelUIScript.h"

CCrosshair::CCrosshair()
	: CScript((UINT)SCRIPT_TYPE::CROSSHAIR)
	, m_CrosshairColor(Vec4(255, 255, 255, 255))
	, m_fSpreadRatio(0.2)
	, m_fBarThikness(2)
	, m_fLength(5)
{
}

CCrosshair::~CCrosshair()
{
}

void CCrosshair::begin()
{
	AppendScriptParam("Spread Ratio", SCRIPT_PARAM::FLOAT, &m_fSpreadRatio, 0.f, 0.f);
	AppendScriptParam("Crosshair Color R", SCRIPT_PARAM::FLOAT, &m_CrosshairColor.x);
	AppendScriptParam("Crosshair Color G", SCRIPT_PARAM::FLOAT, &m_CrosshairColor.y);
	AppendScriptParam("Crosshair Color B", SCRIPT_PARAM::FLOAT, &m_CrosshairColor.z);
	AppendScriptParam("Crosshair Color A", SCRIPT_PARAM::FLOAT, &m_CrosshairColor.w);

	AppendScriptParam("Bar Thikness", SCRIPT_PARAM::FLOAT, &m_fBarThikness);
	AppendScriptParam("Length", SCRIPT_PARAM::FLOAT, &m_fLength);

	// 부모 오브젝트에 패널UI 추가
	SetParentPanelUI();

	CGameObject* pObj = new CGameObject;
	m_pCrossHair	  = new CImageUIScript;

	pObj->SetName(L"Crosshair");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(m_pCrossHair);

	m_pCrossHair->SetUIType(UI_TYPE::CROSSHAIR);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(MESHrect));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	pObj->MeshRender()->GetDynamicMaterial(0);
	pObj->MeshRender()->GetMaterial(0)->SetShader(
		CAssetMgr::GetInst()->Load<CGraphicsShader>(L"GraphicsShader/CrosshairShader.gs"));

	GetOwner()->AddChild(pObj);
}

void CCrosshair::tick()
{
	m_pCrossHair->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_fSpreadRatio);
	m_pCrossHair->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, m_fBarThikness);
	m_pCrossHair->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_2, m_fLength);
	m_pCrossHair->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, m_CrosshairColor);
	m_pCrossHair->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, nullptr);
}

void CCrosshair::SetParentPanelUI()
{
	auto pOwn = GetOwner();

	m_pPanelUI = pOwn->GetScript<CPanelUIScript>();

	if (!m_pPanelUI)
	{
		m_pPanelUI = new CPanelUIScript;
		pOwn->AddComponent(m_pPanelUI);
	}

	m_pPanelUI->DisableMouseInput();
	m_pPanelUI->DisallowDragAndDrop();
	m_pPanelUI->DisallowTexSet();
	m_pPanelUI->SetUIType(UI_TYPE::CROSSHAIR);

	auto meshrender = pOwn->MeshRender();

	if (!meshrender)
	{
		meshrender = new CMeshRender;
		pOwn->AddComponent(meshrender);
	}

	meshrender->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	meshrender->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StaticUIMtrl"), 0);
	meshrender->GetDynamicMaterial(0);
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
